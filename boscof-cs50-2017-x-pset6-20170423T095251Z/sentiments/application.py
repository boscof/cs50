from flask import Flask, redirect, render_template, request, url_for

import helpers
import sys
import os

from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    
    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    
    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, 100)
    
    #declare variables to count the number of positive, negative and neutral tweets
    positive, negative, neutral = 0.0, 0.0, 0.0
    
    for tweet in tweets:
        # analyze tweet
        score = analyzer.analyze(tweet)
        #find the percentage of scores that are positive, negative and neutral
        if score > 0.0:
            positive+=1
        elif score < 0.0:
            negative+=1
        else:
            neutral+=1
    
    #calculate the percentages of positive, negative and neutral tweets
    sum = positive + negative + neutral
    positive/=sum
    negative/=sum
    neutral/=sum
        
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
