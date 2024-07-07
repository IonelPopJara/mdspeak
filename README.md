# mdspeak

`mdspeak` estimates the speaking time of a markdown file based on word count and configurable speaking speeds.

## Installation

To install mdspeak, follow these steps:

1. Clone the repository:

```bash
git clone https://github.com/IonelPopJara/mdspeak.git
cd mdspeak
```

2. Build and install:

```bash
chmod +x autogen.sh
./autogen.sh
./configure
make
sudo make install
```

## Usage

```bash
mdspeak FILE_PATH [OPTIONS]
```

Options:

* `-s <speed>`: Set the speaking speed in words per minute (default is 200).
* `-n`: Set normal speaking speed (150 words per minute).
* `-m`: Set medium speaking speed (175 words per minute).
* `-f`: Set fast speaking speed (200 words per minute).

If no speed is provided, 200 wpm will be used as default.

## Examples

Estimate speaking time of a markdown file:

```bash
# 200 wpm will be used to estimate
mdspeak example.md
```

Estimate speaking time with custom speed:

```bash
mdspeak example.md -s 180
```

## Documentation

The mdspeak man page (mdspeak.1) provides detailed usage instructions and options. You can view it after installation using:

```bash
man mdspeak
```

## Contributing

Contributions are welcome! Here's how you can contribute to mdspeak:

1. Fork the repository and create your branch (git checkout -b feature/my-feature).
2. Commit your changes (git commit -am 'Add new feature').
3. Push to the branch (git push origin feature/my-feature).
4. Create a new Pull Request.

