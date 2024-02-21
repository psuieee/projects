## How to create your own REST API

This is the code participants used to create their own simple REST API using FastAPI with Python.
It defines two GET and two POST requests and implements their backend functionality, which primarily read and write with a CSV file to work as a database of sorts.
Testing is done through the auto-generated FastAPI documentation for the REST API.

### Commands
The following commands were used to setup the project and run the REST API locally.

1) `pip install "uvicorn[standard]"`
2) `uvicorn main:app --reload`
