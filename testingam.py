from flask import Flask, jsonify
import requests

app = Flask(__name__)

@app.route("/jsonparser")
def jsonparser():
    response = requests.get("http://192.168.23.17/")
    json_data = response.json()
    return jsonify(json_data)

if __name__ == "__main__":
    app.run()
