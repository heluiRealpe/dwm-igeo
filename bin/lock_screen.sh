#!/bin/bash

# Set the idle time threshold to 120000 milliseconds (2 minutes)
IDLE_THRESHOLD=100000

while true; do
    # Get the current idle time in milliseconds
    idle_time=$(xssstate -i)

    # Check if the idle time exceeds the threshold
    if [ "$idle_time" -gt "$IDLE_THRESHOLD" ]; then
        # Lock the screen using slock
        slock
    fi

    # Sleep for a short interval before checking again
    sleep 10
done

