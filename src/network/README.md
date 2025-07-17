```
client [TCP socket] --> raw command string --> parser --> Command object
                                                ↓
                                       executor --> result string --> write back
```