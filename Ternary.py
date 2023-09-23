from flask import Flask, redirect, url_for, render_template,request

app = Flask(__name__)

class Node():
    def __init__(self, value=None, endOfWord=False):
        self.value = value
        self.left = None
        self.right = None
        self.equal = None
        self.endOfWord = endOfWord

class Autocomplete():

    def __init__(self, word_list):
        self.n = Node()
        for word in word_list:
            self.insert(word, self.n)

    def insert(self, word, node):
        char = word[0]
        if not node.value:
            node.value = char

        if char < node.value:
            if not node.left:
                node.left = Node()
            self.insert(word, node.left)
        elif char > node.value:
            if not node.right:
                node.right = Node()
            self.insert(word, node.right)
        else:
            if len(word) == 1:
                node.endOfWord = True
                return

            if not node.equal:
                node.equal = Node()
            self.insert(word[1:], node.equal)

    def all_suffixes(self, pattern, node):
        if node.endOfWord:
            yield "{0}{1}".format(pattern, node.value)

        if node.left:
            for word in self.all_suffixes(pattern, node.left):
                yield word
        if node.right:
            for word in self.all_suffixes(pattern, node.right):
                yield word
        if node.equal:
            for word in self.all_suffixes(pattern + node.value, node.equal):
                yield word

    def find(self, pattern):
        final_pattern = {pat: set([]) for pat in pattern}
        for pat in final_pattern.keys():
            word = self.find_(pat)
            if word == None:
                return None
            else:
                completions = {x for x in word}
            return list(completions)

    def find_(self, pattern):
        node = self.n
        for char in pattern:
            while True:
                if char > node.value:
                    node = node.right
                elif char < node.value:
                    node = node.left
                else:
                    node = node.equal
                    break
                if not node:
                    return None
        return self.all_suffixes(pattern, node)

# Driver Function 

#pattern = ['c']
word_list = ['aardvark', 
             'altimeter',
             'apotactic', 
             'bagonet', 
             'boatlip', 
             'carburant', 
             'chyliferous', 
             'consonance', 
             'cyclospondylic', 
             'dictyostele', 
             'echelon', 
             'estadal', 
             'flaunty', 
             'gesneriaceous',
             'hygienic', 
             'infracentral', 
             'jipijapa', 
             'lipoceratous', 
             'melanthaceae',]
words = open("words.txt", "r")

t = Autocomplete(words)
#l = list(t.find(pattern))


#@app.route("/")
#def output():
#    if request.method == 'GET':
#        return render_template("index.html",output = [] )


@app.route("/", methods=["GET", "POST"])
def output():
    if request.method == 'POST':
        text = str(request.form["text"])
        pattern = [text]
        l = list(t.find(pattern))
        return render_template("index1.html",output = l )
    else:
        return render_template("index1.html",output = [] )



if __name__ == "__main__":
	app.run(debug=True)
