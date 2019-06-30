# ltb - LDPL Telegram Bot

This simple LDPL library lets you create a Telegram bot that can receive and send text messages.

It provides two new statements:
* `TG BOT INIT WITH TOKEN <TEXT or TEXT-VAR>`
* `TG BOT SEND MESSAGE <TEXT or TEXT-VAR> TO <NUMBER or NUMBER-VAR>`

And it announces you about new updates via the following sub-procedures that you must implement:
* ltb.onMessage
* ltb.onJoin
* ltb.onDeparture

## Installation

This library requires [LDPL 3.1.0](http://github.com/lartu/ldpl), as it uses many of its new features.

You also need [libcurl](https://curl.haxx.se/libcurl/) with OpenSSL. For example, in Debian, Ubuntu or similar you can install it with:

`$ sudo apt-get install libcurl4-openssl-dev`

## Using ltb

To use `ltb`, you must download this repo and in your LDPL program `IMPORT` the path to `ltb.ldpl`.

To start your bot, use this custom statement passing your bot's [token](https://core.telegram.org/bots#6-botfather):

`TG BOT INIT WITH TOKEN <TEXT or TEXT-VAR>`

This statements will block your main code execution. After you execute it, `ltb` will process each update that your bot receives and calls a sub-procedure passing information about the new update. There are three of them, and you must implement it in your code:

```
# This sub-procedure is called each time a new text message arrives.
# Captions in animation, audio, document, photo, video or voice are captured too.
sub ltb.onMessage
parameters:
    messageId is number     # id of the sent message
    chatId is number        # id of the chat where is was sent
    userData is text map    # user data from sender
    messageText is text     # content of the message
procedure:
    # Your code here
end sub

# This sub-procedure is called each time a user joins a chat.
sub ltb.onJoin
parameters:
    messageId is number     # id of the sent message announcing the user join
    chatId is number        # id of the chat the user joined
    userData is text map    # user data from the user who joined the chat
procedure:
    # Your code here
end sub

# This sub-procedure is called each time a user leaves a chat.
sub ltb.onDeparture
parameters:
    messageId is number     # id of the sent message announcing the user departure
    chatId is number        # id of the chat the user left
    userData is text map    # user data from the user who left the chat
procedure:
    # Your code here
end sub
```

All of the three sub-procedures must be declared with all the parameters specified above, but you may leave a `procedure:` subsection empty if you don't want to do anything on some updates.

The `userData` `TEXT MAP` contains the following information about the user:

|    Key     |          Description          |
|------------|-------------------------------|
| id         | Unique identifier of the user |
| first_name | User's first name             |
| last_name  | User's last name              |
| username   | User's username               |

Bear in mind that some of this elements may be empty (`""`), a message from a channel has no user information for example, and there are users without `last_name` or `username` (but they all have `id` and `first_name`).

You can send messages with this statement:

`TG BOT SEND MESSAGE <TEXT or TEXT-VAR> TO <NUMBER or NUMBER-VAR>`

It takes the message text and the chat id where you want to send it. You must use it after you `INIT` the bot.

See the [examples](examples) to learn more on how to use `ltb`. If you want to test them, paste your bot's token in [token.ldpl](examples/token.ldpl).

## License

ptb is released under the MIT license. It uses the [JSON for Modern C++](https://github.com/nlohmann/json) library by Niels Lohmann, released also under the MIT license.
