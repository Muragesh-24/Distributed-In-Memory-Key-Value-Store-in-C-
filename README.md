# Distributed In-Memory Key-Value Store in C++

A Redis inspired in memory key value store built from scratch in C++.

The goal of this project is to understand and implement core computer science concepts such as hashing, caching, concurrency, networking, persistence, and distributed system design.

---

## Tech Stack

![C++](https://img.shields.io/badge/C++-17/20-blue)
![STL](https://img.shields.io/badge/STL-Data%20Structures-green)
![Networking](https://img.shields.io/badge/Networking-TCP%20Sockets-orange)
![Concurrency](https://img.shields.io/badge/Concurrency-Multithreading-purple)
![Persistence](https://img.shields.io/badge/Persistence-WAL-red)

---

## Why This Project?

Modern backend systems depend heavily on fast key-value stores for:

- Caching
- Session storage
- Rate limiting
- Temporary tokens
- Leaderboards
- Real-time data lookup

This project explores how such systems work internally by implementing the important components manually in C++.

---

## Project Goals

- Build a fast in-memory key-value store using C++.
- Support database-like commands such as `SET`, `GET`, `DELETE`, `EXPIRE`, and `TTL`.
- Implement LRU eviction using efficient data structures.
- Add TTL support so keys can expire automatically.
- Make the store thread-safe for concurrent clients.
- Expose the store through a TCP server.
- Add write-ahead logging so data can be recovered after restart.
- Build a CLI client to interact with the server.
- Benchmark throughput and latency.
- Extend the system toward replication and sharding.

---

## Features

### Core Features

| Feature | Description |
|---|---|
| `SET` | Stores or updates a key-value pair |
| `GET` | Retrieves the value of a given key |
| `DELETE` | Removes a key from the store |
| `EXPIRE` | Sets a time-to-live for a key |
| `TTL` | Returns the remaining lifetime of a key |
| `PING` | Health check command |

### Advanced Features

- LRU cache eviction
- TTL-based key expiry
- Thread-safe concurrent access
- TCP client-server communication
- Write-ahead logging for recovery
- CLI client
- Benchmarking tool
- Primary-replica replication
- Consistent hashing for sharding

---

## Supported Commands

```txt
PING
Response: PONG

SET user:1 muragesh
Response: OK

GET user:1
Response: muragesh

DELETE user:1
Response: OK

EXPIRE user:1 60
Response: OK

TTL user:1
Response: 59

GET unknown:key
Response: NOT_FOUND
```

---

## Architecture

```txt
                 +----------------+
                 |    CLI Client  |
                 +--------+-------+
                          |
                          | TCP
                          |
                 +--------v-------+
                 |   TCP Server   |
                 +--------+-------+
                          |
              +-----------+------------+
              |                        |
      +-------v-------+        +-------v--------+
      | Command Parser|        | Thread Pool    |
      +-------+-------+        +-------+--------+
              |                        |
              +-----------+------------+
                          |
                 +--------v--------+
                 |  KV Store Engine|
                 +--------+--------+
                          |
        +-----------------+-----------------+
        |                 |                 |
+-------v------+  +-------v------+  +-------v------+
| Hash Map     |  | LRU Cache    |  | TTL Manager  |
+--------------+  +--------------+  +--------------+
                          |
                 +--------v--------+
                 | WAL Persistence |
                 +-----------------+
```

---

## Internal Design

### Key-Value Engine

The main storage engine uses an in-memory hash map:

```cpp
unordered_map<string, string> store;
```

Average time complexity:

| Operation | Average Time Complexity |
|---|---|
| `GET` | O(1) |
| `SET` | O(1) |
| `DELETE` | O(1) |

---

### LRU Cache Design

LRU eviction is implemented using a hash map and a doubly linked list.

```cpp
list<pair<string, string>> cacheList;
unordered_map<string, list<pair<string, string>>::iterator> cacheMap;
```

Why this works:

- `unordered_map` provides fast lookup.
- `list` allows moving recently used keys to the front in O(1).
- The least recently used key is removed from the back.

---

### TTL Design

Each key can have an optional expiry timestamp.

```cpp
unordered_map<string, chrono::steady_clock::time_point> expiryMap;
```

The initial version uses lazy expiry. Expired keys are removed when they are accessed.

A later version can include a background cleanup thread.

---

### Write-Ahead Logging

Every write operation is appended to a log file.

Example WAL entries:

```txt
SET name muragesh
SET city kanpur
DELETE city
```

On restart, the server replays the log to rebuild the in-memory state.

Recovery flow:

1. Open the WAL file.
2. Read commands line by line.
3. Replay `SET`, `DELETE`, and expiry operations.
4. Rebuild the in-memory store.

---

### Concurrency Design

The store will support multiple clients concurrently. Shared data structures will be protected using locks.

Planned concurrency tools:

- `mutex` for basic thread safety
- `shared_mutex` for read-heavy workloads
- `condition_variable` for thread pool coordination
- Thread pool to avoid creating a new thread for every request

---

### TCP Server Design

The TCP server receives plain-text commands from clients, passes them to the command parser, executes them on the KV engine, and returns a response.

Example:

```txt
Client sends: SET name muragesh
Server replies: OK

Client sends: GET name
Server replies: muragesh
```

---

## Distributed Extensions

After the single-node version is stable, distributed features can be added.

### Primary-Replica Replication

The primary node receives write requests and forwards them to replica nodes.

```txt
Client
  |
Primary Node
  |
Replica Node
```

### Consistent Hashing

Keys can be distributed across multiple nodes using a hash ring.

```txt
hash(key) -> node
```

This minimizes key movement when nodes are added or removed.

### Sharding

Different nodes can store different partitions of the keyspace.

---

## Roadmap

### Phase 1: Basic In-Memory Store

- [ ] Implement `SET`
- [ ] Implement `GET`
- [ ] Implement `DELETE`
- [ ] Implement command parser
- [ ] Add simple CLI mode

### Phase 2: LRU and TTL

- [ ] Add fixed capacity
- [ ] Implement LRU eviction
- [ ] Add `EXPIRE`
- [ ] Add `TTL`
- [ ] Remove expired keys lazily

### Phase 3: Persistence

- [ ] Add write-ahead log
- [ ] Append write commands to WAL
- [ ] Replay WAL on restart
- [ ] Add recovery tests

### Phase 4: Networking

- [ ] Build TCP server
- [ ] Build TCP client
- [ ] Support multiple client connections
- [ ] Add proper response format

### Phase 5: Concurrency

- [ ] Add mutex-based thread safety
- [ ] Add thread pool
- [ ] Use shared locks for read-heavy workloads
- [ ] Test concurrent clients

### Phase 6: Benchmarking

- [ ] Build benchmark client
- [ ] Measure throughput
- [ ] Measure average latency
- [ ] Measure P95 latency
- [ ] Test read-heavy and write-heavy workloads

### Phase 7: Distributed Extensions

- [ ] Add primary-replica replication
- [ ] Forward writes from primary to replica
- [ ] Add consistent hashing
- [ ] Add multi-node sharding

---

## Possible Folder Structure

```txt
distributed-kv-store-cpp/
│
├── include/
│   ├── KVStore.h
│   ├── LRUCache.h
│   ├── TTLManager.h
│   ├── WAL.h
│   ├── CommandParser.h
│   ├── TCPServer.h
│   └── ThreadPool.h
│
├── src/
│   ├── KVStore.cpp
│   ├── LRUCache.cpp
│   ├── TTLManager.cpp
│   ├── WAL.cpp
│   ├── CommandParser.cpp
│   ├── TCPServer.cpp
│   └── ThreadPool.cpp
│
├── client/
│   └── kv_cli.cpp
│
├── benchmark/
│   └── benchmark.cpp
│
├── tests/
│   ├── test_kvstore.cpp
│   ├── test_lru.cpp
│   └── test_parser.cpp
│
├── data/
│   └── wal.log
│
├── CMakeLists.txt
└── README.md
```

---

## Build and Run

### Build

```bash
mkdir build
cd build
cmake ..
make
```

### Run Server

```bash
./kv_server --port 8080 --capacity 10000
```

### Run Client

```bash
./kv_cli --host 127.0.0.1 --port 8080
```

### Example Client Session

```txt
kv> SET name muragesh
OK

kv> GET name
muragesh

kv> DELETE name
OK

kv> GET name
NOT_FOUND
```

---

## Benchmarking

A custom benchmark tool will be used to test the store under concurrent workloads.

```bash
./benchmark --clients 100 --operations 100000 --read-ratio 80
```

Expected output format:

```txt
Clients: 100
Operations: 100000
Read ratio: 80%
Write ratio: 20%
Throughput: 45000 ops/sec
Average latency: 2.3 ms
P95 latency: 5.8 ms
```

---

## Learning Outcomes

Through this project, I aim to learn and demonstrate:

- C++ system design and clean class structure
- STL-based data structure implementation
- Hash maps and cache eviction policies
- Thread safety and synchronization
- Socket programming and client-server architecture
- File-based persistence and WAL recovery
- Performance benchmarking
- Basics of replication, sharding, and consistent hashing

---

## Current Status

This project is currently under development.

Initial milestone:

- Build a stable single-node key-value store
- Add `SET`, `GET`, and `DELETE`
- Add LRU eviction
- Add TTL support
- Add WAL recovery
- Expose the store using a TCP server

---

## Future Improvements

- RESP-like protocol similar to Redis
- Snapshot-based persistence
- Background TTL cleanup thread
- Replica failover
- Cluster membership
- Consistent hashing with virtual nodes
- Metrics endpoint
- Docker support
- GitHub Actions CI
- Stress testing with large workloads

---

