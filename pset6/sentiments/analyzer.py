import nltk
import sys
import os 

from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        #declare new lists
        self.positives = []
        self.negatives = []
        
        #iterate over every line of the txt file, adding valid words to the list
        with open(positives) as positive:
            for line in positive:
                if not line.startswith((';', ' ')):
                    self.positives.extend(line.split())
        with open(negatives) as negative:
            for line in negative:
                if not line.startswith((';',' ')):
                    self.negatives.extend(line.split())
                    

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        #define tokenizer and create a list that divides the original string into substrings
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        score = 0
        
        #iterate over each token, determining if each word is positive,negative or neutral
        for token in tokens:
            temp = token.lower()
            #increase score by 1 if word is positive
            if temp in self.positives:
                score+=1
            #decrease score by 1 if word is negative
            elif temp in self.negatives:
                score-=1
        return score
