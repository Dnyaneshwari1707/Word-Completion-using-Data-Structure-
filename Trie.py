"""
A fast data structure for searching strings with autocomplete support.
"""
import os
from functools import reduce
from flask import Flask, redirect, url_for, render_template,request

app = Flask(__name__)

class Trie(object):
    def __init__(self):
        self.children = {}
        self.flag = False # Flag to represent that a word ends at this node

    def add(self, char):
        self.children[char] = Trie()

    def insert(self, word):
        node = self
        for char in word:
            if char not in node.children:
                node.add(char)
            node = node.children[char]
        node.flag = True

    def contains(self, word):
        node = self
        for char in word:
            if char not in node.children:
                return False
            node = node.children[char]
        return node.flag

    def all_suffixes(self, prefix):
        results = set()
        if self.flag:
            results.add(prefix)
        if not self.children: return results
        return reduce(lambda a, b: a | b, [node.all_suffixes(prefix + char) for (char, node) in self.children.items()]) | results

    def autocomplete(self, prefix):
        node = self
        for char in prefix:
            if char not in node.children:
                return set()
            node = node.children[char]
        return list(node.all_suffixes(prefix))


words = open("words.txt", "r")
x = Trie()
for i in words:
    x.insert(i)
#a = input("Enter prefix :")
#t = x.autocomplete(a)
#print(t)

@app.route("/", methods=["GET", "POST"])
def output():
    if request.method == 'POST':
        text = str(request.form["text"])
        l = list(x.autocomplete(text))
        return render_template("index1.html",output = l )
    else:
        return render_template("index1.html",output = [] )


if __name__ == "__main__":
	app.run(debug=True)

