#!/bin/bash

echo "Running a micro php web server to host/test the web client and its demo content..."

php -S 0.0.0.0:8080 -t .
