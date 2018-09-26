from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #find the details of the stocks owned
    stocks = db.execute("SELECT * FROM portfolio WHERE id=:id", id=session["user_id"])
    
    #declare a variable to calculate the net worth of the portfolio
    portfolio_balance=0
    
    #update the stocks
    for stock in stocks:
        #find symbol
        symbol = stocks[0]["symbol"]
        #find number of shares owned
        shares = stocks[0]["shares"]
        #look up stock
        quote = lookup(symbol)
        #update table
        db.execute("UPDATE portfolio SET price=:price, total=:total WHERE id=:id",price=usd(quote["price"]), \
        total=usd(quote["price"] * shares), id=session["user_id"])
        #take the cumulative sum of each stock's net worth
        portfolio_balance+=quote["price"]*shares
        
    #find the cash balance
    balance = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    cash = balance[0]["cash"]
    
    #find the total balance
    total = portfolio_balance + cash
    
    #render the table
    return render_template("index.html", stocks=stocks, cash=usd(cash), total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        #look up symbol
        quote = lookup(request.form.get("symbol"))
        #check that the stock exists
        if not quote:
            return apology("Please enter a valid symbol")
            
        #check that the input is valid
        shares = int(request.form.get("shares"))
        if shares<0:
            return apology("Please enter a valid number of shares")
        
        #obtain the balance
        balance = db.execute("SELECT cash FROM users where id = :id", id=session["user_id"])
        
        #check that there is enough cash
        if float(balance[0]['cash']) < quote["price"] * shares:
            return apology("Insufficient funds")
        
        #update cash balance
        db.execute("UPDATE users SET cash = cash-:cost WHERE id=:id", cost=(quote["price"]*shares), id=session["user_id"])
        
        #check if the shares are already owned
        owned_shares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"],\
        symbol=quote["symbol"])
        
        #update the existing portfolio
        if not owned_shares:
            db.execute("INSERT INTO portfolio (name, symbol, shares, price, total, id) VALUES(:name, :symbol, :shares, :price, :total, :id)",\
            name=quote["name"], symbol=quote["symbol"], shares=shares, price=usd(quote["price"]), total=usd(quote["price"] * shares )\
            ,id=session["user_id"])
        else:
            total_shares = owned_shares[0]["shares"] + shares
            db.execute("UPDATE portfolio SET shares=:shares, total=:total WHERE id=:id AND symbol=:symbol", \
            shares=total_shares, total=usd(total_shares*quote["price"]), id=session["user_id"], symbol=quote["symbol"])
        
        #record transaction in history
        db.execute("INSERT INTO history (name, symbol, shares, price, id) VALUES(:name, :symbol, :shares, :price, :id)",\
        name=quote["name"], symbol=quote["symbol"], shares=shares, price=usd(quote["price"]), id=session["user_id"])
         
        #redirect user to the home page
        return redirect(url_for("index"))    
    else:
        #display form
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    #obtain the details of the transaction
    transactions=db.execute("SELECT * FROM history WHERE id=:id", id=session["user_id"])
    #display table
    return render_template("history.html", transactions=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        # else if user reached route via GET (as by clicking a link or via redirect)
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method =="POST":
        #look up the symbol
        quote = lookup(request.form.get("quote"))
        #check that the stock exists
        if not quote:
            return apology("Please enter a valid symbol")
        #display quote
        return render_template("quoted.html", quote=quote)
    else:
        #display form
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        #check that username and password is not empty
        if request.form.get("username")=="":
            return apology("Missing username!")
        if request.form.get("password")=="" or request.form.get("confirm password")=="":
            return apology("Missing password!")
        if request.form.get("password")!=request.form.get("confirm password"):
            return apology("Passwords do not match!")
            
        #encrypt the password
        hash = pwd_context.encrypt(request.form.get("password"))  
        
        #register the username and password into the database
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username,:hash)", username=request.form.get("username"), hash=hash)
        
        #check if the registration has been successful
        if not result:
            return apology("Username already exists")
            
        #remember which user has logged in
        session["user_id"]=result
        
        #redirect user to home page
        return redirect(url_for("index"))
            
    else:
        #display registration page
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method=="POST":
        #declare temporary variables to store the stock and the number of shares to be sold
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        
        #check that the input is valid
        quote = lookup(symbol)
        if not quote:
            return apology("Please enter a valid symbol")
        if shares<0:
            return apology("Number of shares sold must be positive")
        
        #obtain the number of shares owned
        portfolio_shares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", \
        id=session["user_id"], symbol=symbol)
        
        #check that the user owns enough shares to sell 
        if not portfolio_shares:
            return apology("You must own the stock to sell")
        if shares > int(portfolio_shares[0]["shares"]):
            return apology("You don't own enough shares!")
        
        #calculate the money received from selling the stock and the remaining shares
        money = shares*quote["price"]
        remaining_shares = portfolio_shares[0]["shares"] - shares
        
        #update portfolio
        if remaining_shares==0:
            db.execute("DELETE FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"], symbol=symbol)
        else:
            db.execute("UPDATE portfolio SET shares=:shares, price=:price, total=:total WHERE id=:id AND symbol=:symbol", \
            shares=remaining_shares, price=usd(quote["price"]), total=usd(remaining_shares*quote["price"]), symbol=symbol,\
            id=session["user_id"])
        
        #update cash balance
        db.execute("UPDATE users SET cash=cash + :money WHERE id=:id", money=money, id=session["user_id"])
        
        #record in transaction history
        db.execute("INSERT INTO history (name, symbol, shares, price, id) VALUES(:name,:symbol,:shares,:price,:id)",\
        name=quote["name"], symbol=quote["symbol"], shares=-shares, price=usd(quote["price"]), id=session["user_id"])
        
        #redirect user to home page
        return redirect(url_for("index"))
        
    else:
        #display form
        return render_template("sell.html")
        
@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change user password."""
    if request.method=="POST":
        #check that none of the fields are empty
        if request.form.get("old password")=="":
            return apology("Please enter in your old password")
        if request.form.get("new password")=="" or request.form.get("confirm password")=="":
            return apology("Please enter in your new password")
        if request.form.get("new password")!=request.form.get("confirm password"):
            return apology("New passwords do not match!")
            
        # query database for id
        rows = db.execute("SELECT * FROM users WHERE id=:id", id=session["user_id"])
        # ensure password is correct
        if not pwd_context.verify(request.form.get("old password"), rows[0]["hash"]):
            return apology("The old password is incorrect")
            
        #encrypt the new password
        hash = pwd_context.encrypt(request.form.get("new password"))  
        
        #register the username and password into the database
        db.execute("UPDATE users SET hash=:hash WHERE id=:id", hash=hash, id=session["user_id"])
        
        #redirect user to home page
        return render_template("successful.html")
        
    else:
        #display form to change password
        return render_template("change.html")