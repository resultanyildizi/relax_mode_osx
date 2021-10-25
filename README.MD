# OSX Relax Mode

Relax Mode is a command tool that sets a shutdown timer based on a given amount of time, opens a relaxing youtube video in full screen mode in Safari, and sets the computer's volume to a low level.

## Usage

To run:

```
    # You can specify hours and minutes with -h (or --hour) and -m (or --minute) parameters
    ./relax_mode -h <hours> -m <minutes>

    # You can also set the timer by just entering the minutes. The default <hours> value is 0.
    ./relax_mode -m <minutes>

    # You can also set the timer by just entering the hours. The default <minutes> value is 0.
    ./relax_mode -h <hours>
```

To stop:

```
    # You can stop all shutdown timers by entering --stop flag.
    ./relax_mode --stop
```
