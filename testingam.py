import flask
import requests

app = flask.Flask(__name__)
context = ("../certs/cert.pem", "../certs/pkey.pem")
@app.route("/jsonparser")
def jsonparser():
    response = requests.get("http://192.168.23.17/")
    json_data = response.json()
    cliresp = flask.jsonify(json_data)
    cliresp.headers.add('Access-Control-Allow-Origin', '*')
    return cliresp

if __name__ == "__main__":
    app.run(threaded=True, host="0.0.0.0" , ssl_context=context) ##  ssl_context="adhoc")

