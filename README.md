# Striker (C++)

Striker is a high-performance Blackjack simulation and analysis tool written in C++. It supports multiple player strategies and deck configurations, and is designed for multi-threaded execution on modern hardware.

> This project is part of a multi-language software engineering demonstration. See the full collection at [rees.me](http://rees.me).

## Features

- Player strategy support:
  - Mimic dealer
  - Basic strategy
  - Linear regression
  - Polynomial regression
  - Neural network-based strategy
  - High-low counting
  - Wong counting
- Deck options:
  - Single-deck
  - Double-deck
  - Six-deck shoe
- Fast, parallelized simulation using multiple threads
- Configurable via command-line arguments
- Outputs simulation results as JSON (easy to analyze or visualize)

## Usage

### Build

```bash
make all
```

### Run (direct binary)

```bash
bin/striker-plus [options]
```

#### Options

| Option                              | Description                                           |
|-------------------------------------|-------------------------------------------------------|
| `--help`                            | Show this help message                                |
| `--version`                         | Display the program version                           |
| `-h`, `--number-of-hands` `<n>`     | Number of hands to simulate                           |
| `-t`, `--number-of-threads` `<n>`   | Number of threads to use                              |
| `-M`, `--mimic`                     | Use the mimic dealer player strategy                  |
| `-B`, `--basic`                     | Use the basic player strategy                         |
| `-L`, `--linear`                    | Use the linear regression player strategy             |
| `-P`, `--polynomial`                | Use the polynomial regression player strategy         |
| `-N`, `--neural`                    | Use the neural network player strategy                |
| `-H`, `--high-low`                  | Use the high-low count player strategy                |
| `-W`, `--wong`                      | Use the Wong count player strategy                    |
| `-1`, `--single-deck`               | Use a single deck of cards and rules                  |
| `-2`, `--double-deck`               | Use a double deck of cards and rules                  |
| `-6`, `--six-shoe`                  | Use a six-deck shoe of cards and rules                |

#### Example

```bash
bin/striker-plus -B -2 -h 100000 -t 8
```

Runs a simulation using the **basic strategy** on a **double-deck** game for **100,000 hands** using **8 threads**.

### Run with Makefile Aliases

#### Single-deck simulations

```bash
make rm1   # Mimic strategy, single deck
make rl1   # Linear strategy, single deck
make rp1   # Polynomial strategy, single deck
make rn1   # Neural strategy, single deck
make rb1   # Basic strategy, single deck
make rh1   # High-low strategy, single deck
make rw1   # Wong strategy, single deck
```

#### Double-deck simulations

```bash
make rm2
make rl2
make rp2
make rn2
make rb2
make rh2
make rw2
```

#### Six-deck simulations

```bash
make rm6
make rl6
make rp6
make rn6
make rb6
make rh6
make rw6
```

### Run all simulations

```bash
make run-all
```

Runs every combination of player strategy and deck configuration.

## Project Structure

```
.
├── src/
│   ├── arguments/       # Command-line argument parsing
│   ├── cards/           # Card, deck, and hand logic
│   ├── constants/       # Game and simulation constants
│   ├── table/           # Table management and game flow
│   ├── simulator/       # Core simulation logic
│   ├── aws/             # Optional AWS integration (e.g., logging or metrics)
│   └── main.cpp         # Entry point
├── obj/                 # Compiled object files
├── bin/                 # Compiled executable: striker-plus
├── Makefile             # Main build file
├── Makefile.run         # Extended run targets for strategy/deck combinations
└── README.md
```

## Requirements

- g++ with C++20 support
- GNU Make
- Libraries:
  - `cjson`
  - `libcurl`
  - `libmongoc-1.0`
  - `libbson-1.0`
  - `pthread`
  - `uuid`

## Installation

To install the binary to your local Striker bin directory:

```bash
make install
```

Binary will be copied to:

```bash
~/Striker/bin/striker-plus
```

## License

This project is open source and available under the [MIT License](LICENSE).

## Author

**Wade Rees**  
📫 [wade@rees.me](mailto:wade@rees.me)  
🔗 [LinkedIn](https://www.linkedin.com/in/wade-rees-978a02)  
🌐 [http://rees.me](http://rees.me)

