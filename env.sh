docker stop eval_protocol
docker rm eval_protocol
docker run --name eval_protocol -itd   -v $(pwd):/home  -e LOCAL_USER_NAME=$USER -e LOCAL_USER_ID=`id -u $USER` zizdlp/gperf tail -f /etc/hosts
docker exec -it eval_protocol bash