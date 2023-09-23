from flask import Flask, redirect, url_for, render_template,request

app = Flask(__name__)

def binary_search(arr: list, target: str):
    start = 0
    end = len(arr) - 1

    while start <= end:
        middle = start + ((end - start) // 2)
        midpoint = arr[middle]
        if midpoint.startswith(target):
            curr_el = middle
            while target in midpoint:
                midpoint = arr[curr_el]
                curr_el -= 1
            possible_words = []
            for i in range(2, 100):
                midpoint = arr[curr_el + i]
                if midpoint.startswith(target):
                    possible_words.append(midpoint)
            return possible_words
        if midpoint > target:
            end = middle - 1
        elif midpoint < target:
            start = middle + 1
        else:
            return midpoint
    return 'Nothing found for your search'

dictionary_file = open("words.txt", "r")
dictionary_arr = dictionary_file.read().splitlines()
dictionary_arr.sort()

#print("Enter the word: ")
#inputText = input()
#l = binary_search(dictionary_arr, inputText)
#print(l)

@app.route("/", methods=["GET", "POST"])
def output():
    if request.method == 'POST':
        text = str(request.form["text"])
        pattern = [text]
        l = list(binary_search(dictionary_arr, text))
        return render_template("index1.html",output = l )
    else:
        return render_template("index1.html",output = [] )


if __name__ == "__main__":
	app.run(debug=True)

