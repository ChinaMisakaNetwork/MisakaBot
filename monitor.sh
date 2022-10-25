 #!/bin/sh
    while :
    do
    echo "Current DIR is " $PWD
    stillRunning=$(ps -ef |grep "$PWD/MisakaBot" |grep -v "grep")
    if [ "$stillRunning" ] ; then
    echo "MisakaBot was already started by another way"
    echo "Kill it and then startup by this shell, other wise this shell will loop out this message annoyingly"
    kill -9 $pidof $PWD/Misakabot
    else
    echo "MisakaBot was not started"
    echo "Starting MisakaBot ..."
    $PWD/MisakaBot
    echo "MisakaBot was exited!"
    fi
    sleep 10
    done