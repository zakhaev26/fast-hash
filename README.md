<p align="center">
  <img src="media/banner-high-res.png" alt="FastHash Banner" width="100%">
</p>

<p align="center">FastHash is a simple, fast in-memory key-value store written in C++.</p>

- [x] Core Features (Completed)
  - [x] Basic key-value `SET` and `GET` commands
  - [x] `DEL` command to delete keys
  - [x] `EXPIRE` command to set TTL (time-to-live) on keys
  - [x] `TTL` command to get remaining TTL of keys
  - [x] `SETEX` command (set key with expiry in one command)
  - [x] `EXISTS` command to check presence of keys
  - [x] `KEYS` command to list all keys (with pattern support planned)
  - [x] `FLUSHALL` command to clear all keys in the store
  - [x] `SAVE` command to persist the current key-value store snapshot to disk (via main thread - to be fixed) (JSON format)
  - [x] `LOAD` command to load persisted data from disk on startup
  - [ ] In-House Primary Data Structures


- [ ] Persistence (Planned~WIP)
  - [ ] Background snapshot saving to avoid blcking the main thread
  - [ ] AOF logging for durability and incremental persistence
  - [ ] Periodic snapshotting combined with AOF for crash recovery
  - [ ] Configurable persistence options (enable/disable, frequency)
  
- [ ] Networking Layer (In Progress)
  - [x] TCP server using standalone ASIO (Maybe In house TCP Server ? Look into it) <--*
  - [ ] Async multi-client handling with full concurrency support
  - [ ] Command parsing over network (line-based, later RESP protocol)
  - [ ] Well-structured, production-grade request-response lifecycle
  - [ ] Client session management, connection cleanup, error handling
  - [ ] TLS/SSL support for encrypted connections (planned)

- [ ] Advanced Data Types and Commands (Planned)
  - [ ] Hashes (maps/dictionaries) as value types (https://redis.io/technology/data-structures/) <--*
  - [ ] Lists, Sets, Sorted Sets (ZSets)
  - [ ] INCR/DECR numeric commands
  - [ ] Transactions and Lua scripting support

- [ ] SRE (Planned)
  - [ ] Replication (master-slave wala)
  - [ ] Persistence guarantees and durability levels
  - [ ] Cluster mode with sharding and failover
  - [ ] Monitoring and metrics (Prometheus integration)

- [ ] Tooling & Developer Experience (Planned)
  - [ ] test suites
  - [ ] docs and examples
