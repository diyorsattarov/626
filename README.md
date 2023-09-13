# cpp-irc-bot

Twitch IRC Bot using Boost ASIO and Websockets


## Table of Contents

- [cpp-irc-bot](#cpp-irc-bot)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
  - [Usage](#usage)
  - [Features](#features)
  - [Contributing](#contributing)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Introduction

This project is a Twitch IRC bot implemented in C++ using Boost ASIO and Websockets. It allows you to connect to a Twitch channel's chat, authenticate using OAuth2 tokens, and interact with the chat by responding to commands and messages.

## Getting Started

To get started with this Twitch IRC bot, follow the steps below:

### Prerequisites

Before using this bot, you need to have the following prerequisites installed on your system:

- C++ compiler (supporting C++17 or later)
- Boost C++ Libraries (Boost ASIO and Boost Beast)
- CMake (for building the project)
- Twitch account with a registered application (for obtaining OAuth2 tokens)
- Create a config.json file with your Twitch client secret.

### Installation

Clone the repository to your local machine.

```
git clone https://github.com/diyooor/cpp-irc-bot.git
```

Build the project using CMake. Navigate to the project's root directory and create a build directory:
```
mkdir build
cd build
```

Configure the project with CMake:

```
cmake ..
```

Build the executable:

```
make
```

Run the bot:

```
./test_main.exe
```

## Usage

Once the bot is running, it will connect to the specified Twitch channel's chat and listen for messages. You can extend the code to implement custom message handling logic, such as responding to specific commands.

## Features

- Connects to Twitch chat using WebSocket.
- Authenticates with Twitch using OAuth2 tokens.
- Joins a specific Twitch channel's chat.
- Listens for messages and can respond to commands (e.g., !dice). (not working)

## Contributing

Contributions to this project are welcome! If you'd like to contribute, please follow these guidelines:

- Fork the repository.
- Create a new branch for your feature or bug fix.
- Make your changes and commit them with descriptive messages.
- Create a pull request to submit your changes.
- If you find any issues or have suggestions, please open an issue in the project's GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE.md) - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Boost C++ Libraries for providing the ASIO and Beast libraries.
- The Twitch API for enabling bot authentication and chat interaction.
- The open-source community for valuable contributions and inspiration.