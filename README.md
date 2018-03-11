<<<<<<< HEAD
# NoSqlDb Key/Value Database (C++)
### A non-relational database for use in a source code repository is implemented in C++ using the facilities of the standard C++ Libraries and Visual Studio 2017. It also uses the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management.

- [x] Package prologues and function prologues are included.
- [x] The public interface documentation — holding a short description of the package operations, the required files to build the package, and the build process and the maintenance history — is included.
- [x] Each funtion includes some comments to describe its operation.

1. support addition and deletion of key/value pairs.
1. support editing of values including the addition and/or deletion of relationships, editing text metadata, and replacing an existing value's instance with a new instance. Editing of keys is forbidden.
1. support queries for:
   1. The value of a specified key.
   1. The children of a specified key.
   1. The set of all keys matching a specified regular-expression pattern.
   1. All keys that contain values written within a specified time-date interval. If only one end of the interval is provided, it shall take the present as the other end of the interval.
1. support queries on the set of keys returned by a previous query3, e.g., an "and"ing of multiple queries. Also support queries on the union of results of one or more previous queries, e.g., an "or"ing of multiple queries.

> Each package is accompanied with a test stub in which it is the piece of code used to test each package against different cases, and it is done independently.
=======
# NoSqlDb
NoSqlDb Key/Value Database (C++)
>>>>>>> 9076f669a8122d56b8d941a68fbbcb2a61f338cb
