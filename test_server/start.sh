echo "(Shell) Expected Command: "
echo "(Shell) >> docker run -i --rm -p 8000:8000 -p 8765:8765 --env "PYTHONUNBUFFERED=1" philipdavis82/visgui.test"
# echo "(Shell) "

start=`date +%s`


echo "(Shell) Starting 'test_server.py'"
python3 test_server.py &
echo "(Shell) Starting 'test_websocket.py'"
python3 test_websocket.py &

# echo "(Shell) "
echo "(Shell) Press Enter To Exit"
# echo "(Shell) "

read -n1 ans

echo "(Shell) Stopping 'test_server.py'"
pkill "python3 test_server.py"
echo "(Shell) Stopping 'test_websocket.py'"
pkill "python3 test_websocket.py"

end=`date +%s`
duration=$((end-start))


if [ $duration -lt 1 ]; then
    # echo "(Shell) "
    echo "(ERROR) Container Terminated Early... "
    echo "(ERROR) Try running docker container with '-i' ie. 'docker -i <image>'"
    # echo "(Shell) "
fi

