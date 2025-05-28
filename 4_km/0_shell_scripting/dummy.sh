#!/bin/sh

echo hello

sleep 2

exit_code=$?
echo $exit_code

echo venkat

sleep 3 

exit_code=$?
echo $exit_code

echo odela


sleep 10 &

exit_code=$?
echo $exit_code

echo sid