# TLS-ish

A small project where I'm trying to understand how TLS works by building some of the pieces myself.

Right now, only the **key exchange part** (ECDH) has been implemented. The actual encrypted communication, authentication, etc. are not implemented yet.

The project uses:

- **C++ sockets API** for the networking part. This currently uses POSIX/Unix socket headers and APIs, so the code is **OS-dependent** and is mainly intended for Linux/Unix systems.
- **libsodium** for generating the public/private key pair and deriving the shared session keys.
- TCP for communication between the client and server.

## Message framing

Each message currently contains a 1-byte type, a 4-byte length field, and the payload.

The length is sent in network byte order, and the code has `send_all()` / `recv_exact()` functions to make sure the requested amount of data is actually sent/received.

## Key Sharing

Each side generates its own public/private key pair using libsodium. The public key can be sent over the connection, while the private key stays local.

After exchanging public keys, each side uses its own private key and the other side's public key to derive session keys. The resulting keys are then used as the basis for secure communication.

The key generation and session-key derivation are handled using libsodium's `crypto_kx` API.

## Current state

Implemented:

- TCP client/server
- Basic 1-to-1 communication
- Message framing
- Public/private key generation
- Public key exchange
- Shared session key derivation
- Basic handling of different message types
- Multiple client handling using threads (2)

The server currently keeps track of two clients and forwards messages between them.

Not implemented yet:

- Actual message encryption
- Authentication
- Certificates / certificate verification
- HMAC / MAC
- Finished messages
- Proper TLS-style handshake state machine
- Replay protection
- Proper error handling in a lot of places

So this is **not an implementation of TLS**. It's mainly a learning project.

## Building

You need:

- C++ compiler
- libsodium
- Linux/Unix-like OS

Example:

```bash
g++ server.cpp messenger.cpp crypto.cpp -lsodium -o server
g++ client.cpp messenger.cpp crypto.cpp -lsodium -o client
```

Then run the server and connect with the client.

## Why is is still on level 2?

Coding everything in Cpp took too long, a lot of time was spent on learning to make a socket connection itself.
A lot of errors, byte related issues had to be handled manually which again caused a lot of delay
