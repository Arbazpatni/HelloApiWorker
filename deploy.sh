# Pass username of the docker account as the first argument.
username=$1

# write the current time to config.h file
echo "Re Generating config.h file..."
date=$(date)
echo 'char *Build_Time = "'$date'";' > config.h

# build the dockerfile
echo "Building docker Image..."
docker build -t $username/helloapiworker:latest $username/helloapiworker:v2 .

# push the images
echo "Pushing docker Image..."
docker push $username/helloapiworker:latest
docker push $username/helloapiworker:v2

echo "Done!"
