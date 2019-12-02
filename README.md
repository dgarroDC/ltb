![The LDPL Telegram Bot Library](images/ldpl-telegram-logo.png)

**The LDPL Telegram Bot Library** is a simple LDPL library lets you create a Telegram bot that can receive and send text messages.

It provides some new statements:
* `TG BOT INIT WITH TOKEN <TEXT or TEXT-VAR>`
* `TG BOT SEND MESSAGE <TEXT or TEXT-VAR> TO <NUMBER or NUMBER-VAR> [WITH INLINE KEYBOARD <TEXT or TEX-VAR>]`
* `TG BOT CREATE INLINE KEYBOARD IN <TEXT-VAR>`
* `TG BOT ADD ROW TO INLINE KEYBOARD <TEXT-VAR>`
* `TG BOT ADD BUTTON WITH TEXT <TEXT or TEXT-VAR> AND {URL | CALLBACK DATA} <TEXT or TEXT-VAR> TO INLINE KEYBOARD <TEXT-VAR>`

And it announces you about new updates via the following sub-procedures that you must implement so you can handle them if you want:
* `ltb.onMessage`
* `ltb.onJoin`
* `ltb.onDeparture`
* `ltb.onCallbackQuery`

## Installation

You can install this library **by hand** or **using [LPM](https://github.com/Lartu/ldpl#-libraries)**.

### 📦 Installing using LPM

Open a terminal and write `lpm install ltb`. Once downloaded, include it in your LDPL project by adding the line:

```coffeescript
using package ltb
```

before the `data` and `procedure` sections of your source file. The library is ready to be used.

### ✋🏻 Installing by hand

Include the library into your LDPL project by copying the file *ltb.ldpl* and the folders *extensions* and *thirdparty* to your project directory and then adding the line:

```coffeescript
include "ltb.ldpl"
```

before the `data` and `procedure` sections of your source file. The library is
ready to be used.

### ⚠️ Note

This library requires [LDPL 4.3](http://github.com/lartu/ldpl).

You also need [libcurl](https://curl.haxx.se/libcurl/) with OpenSSL. For example, in Debian, Ubuntu or similar you can install it with:

`$ sudo apt-get install libcurl4-openssl-dev`

## Using ltb

To use `ltb`, you must download this repo and in your LDPL program `IMPORT` the path to `ltb.ldpl`.

To start your bot, use this custom statement passing your bot's [token](https://core.telegram.org/bots#6-botfather):

`TG BOT INIT WITH TOKEN <TEXT or TEXT-VAR>`

This statement will block your main code execution, unless there is a problem initializing the bot (network problem, invalid token, etc.), in which case it will set `ERRORCODE` in `1`, `ERRORTEXT` with the error description and return.

After the bot is successfully initialized, `ltb` will process each update that your bot receives and calls a sub-procedure passing information about the new update. There are four of them, and you must implement it in your code:

### `ltb.onMessage`
```coffeescript
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
```

### `ltb.onJoin`
```coffeescript
# This sub-procedure is called each time a user joins a chat.
sub ltb.onJoin
parameters:
    messageId is number     # id of the sent message announcing the user join
    chatId is number        # id of the chat the user joined
    userData is text map    # user data from the user who joined the chat
procedure:
    # Your code here
end sub
```

### `ltb.onDeparture`
```coffeescript
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

### `ltb.onCallbackQuery`
```coffeescript
# This sub-procedure is called each time a user press a button with callback data.
sub ltb.onCallbackQuery
parameters:
    chatId is number        # id of the chat from the message of the button that was pressed
    userData is text map    # user data from the user who pressed the button
    callbackData is text    # callback data from the pressed button
    answerText is text      # store a text here to display a notification to the user
    answerAlert is number   # store 1 here to show answerText as an alert instead of a notification at the top of the chat screen
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

`TG BOT SEND MESSAGE <TEXT or TEXT-VAR> TO <NUMBER or NUMBER-VAR> [WITH INLINE KEYBOARD <TEXT or TEX-VAR>]`

It takes the message text and the chat id where you want to send it. You must use it after you `INIT` the bot. If there is a problem delivering the message (network problem, invalid chat id, etc.), `ltb` will set `ERRORCODE` in `1` and `ERRORTEXT` with the error description.

You can also optionally send the message with an inline keyboard. The `TEXT` you pass here must be a serialized JSON of a [InlineKeyboardMarkup object](https://core.telegram.org/bots/api#inlinekeyboardmarkup), but don't worry, you can construct it with `ltb` statements:

`TG BOT CREATE INLINE KEYBOARD IN <TEXT-VAR>`

This statement stores in the variable an inline keyboard with an empty row.

`TG BOT ADD ROW TO INLINE KEYBOARD <TEXT-VAR>`

This statement modifies the inline keyboard appending an empty row at the end

`TG BOT ADD BUTTON WITH TEXT <TEXT or TEXT-VAR> AND {URL | CALLBACK DATA} <TEXT or TEXT-VAR> TO INLINE KEYBOARD <TEXT-VAR>`

This statement modifies thee inline keyboard appending a button at the end of its last row. You must specify the the text of the button's label and:
* an URL that will be opened when an user press the button, or
* callback data that will be send to `ltb.onCallbackQuery` and lets you handle and answering it.

**Warning**: The last two statements are only guaranteed to work with inline keyboards that you created and modified with `ltb` statements. That means that if you pass some `InlineKeyboardMarkup` that you made manually or with some other tools, the statements may modify them in such a way that the text becomes an invalid `InlineKeyboardMarkup`.

All `ltb` errors are logged to the *standard error*.

See the [examples](examples) to learn more on how to use `ltb`. If you want to test them, paste your bot's token in [token.ldpl](examples/token.ldpl).

## License

`ltb` is released under the MIT license. It uses the [JSON for Modern C++](https://github.com/nlohmann/json) library by Niels Lohmann, released also under the MIT license.
