from flask import Flask, redirect, url_for, render_template,request

app = Flask(__name__)

#Trie Data Structure

class trieNode:
    def __init__(self):
        self.next = {}
        self.leaf = False
    def add_item(self, item):
        i = 0
        while i < len(item):
            k = item[i]
            if not k in self.next:
                node = trieNode()
                self.next[k] = node
            self = self.next[k]
            if i == len(item) - 1: 
                self.leaf = True
            else:
                self.leaf = False
            i += 1
    def search(self, item):
        if self.leaf and len(item) == 0:
            return True
        first = item[:1]  
        str = item[1:]  
        if first in self.next:
            return self.next[first].search(str)
        else:
            return False
    def traversal(self, item):
        if self.leaf:
            print (item)
        for i in self.next:
            s = item + i
            self.next[i].traversal(s)
    def autocomplete(self, item):
        i = 0
        s = ''
        while i < len(item):
            k = item[i]
            s += k
            if k in self.next:
                self = self.next[k]
            else:
                return 'NOT FOUND'
            i += 1
        self.traversal(s)
        return 'END'


with open('words.txt') as wordFile:
    wordList = wordFile.read().split()
x = trieNode()
for i in wordList:
    x.add_item(i)
    
#print(x.autocomplete(input("Enter starting letters : ")))

@app.route("/", methods=["GET", "POST"])
def output():
    if request.method == 'POST':
        text = request.form["text"]
        l = x.autocomplete(text)
        return render_template("index1.html",output = l )
    else:
        return render_template("index1.html",output = [] )

if __name__ == "__main__":
    app.run(debug=True)
