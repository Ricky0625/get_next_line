# Get Next Line

> Reading a line from a fd is way too tedious. Program a function that returns a line read from a file descriptor.

## Function description

**Function name**   : `get_next_line`

**Prototype**       : `char *get_next_line(int fd)`

**Parameters**      : `fd`: The file descriptor to read from

**Return value**    : Read line: correct behavior. `NULL`: there is nothing else to read, or an error occured.

**Description**     : Write a function that returns a *line read* from a file descriptor

Some behavior of the function according to the pdf.

1. If we call `get_next_line` in a loop, it should let us read the text file pointed by the file descriptor, **one line at a time**. *What's a line?*
2. The function should return **the line that was read**. If there is *nothing else to read* or *if an error occurred*, should return `NULL`. *What are the errors?*
3. The function should be working when reading from the standard input. `fd = 0`.
4. The returned line **should include the terminating `\n` character** except if the end of file (EOF) was reached and does not end with a `\n` character. *What the difference between `\0` and `EOF`*?

Desire output: Able to read the entire file by calling `get_next_line` on a loop.

Read as little as possible each time `get_next_line()` is called. If encounter a new line, have to return the current line. Don't read the whole file and then process each line.

## Learning resources

### File descriptor

Resource: [File Descriptor](https://www.computerhope.com/jargon/f/file-descriptor.htm#:~:text=A%20file%20descriptor%20is%20a,Grants%20access.)

File descriptor is a reference that points to a file that is open on your computer. So, when we open a file, the operating system will create an entry in the global file table. The table will contains info such as the fd, access restrictions (read-only, write-only), and its location. Usually the file descriptor will have a non-negative integer values and there will be at least one fd exists for every open file on the system.

On a Unix-like OS, the first three file descriptors, are reserved.
0: standard input
1: standard output
2: standard error

File descriptor is a unique identifer for a file or other I/O resource, such as a pipe or network socket. It describes a data resource, and how that resource can be accessed. When a program asks to open a file or other data resource like network socket, the kernel will:

1. Grant access
2. Creates an entry in the global file table
3. Provides the software with the location of that entry

> A global file table contains info such as the inode of the file, byte offset, and the access restrictions for that data stream.

File descriptors typically have **non-negative integer values**. At least one file descriptor exists for every open file on the system.

File descriptor is a reference that points to a file that is open on the system.

![Process](https://www.computerhope.com/jargon/f/file-descriptor.jpg)

#### Default file descriptors

| Name | File descriptor | Description | Abbreviation |
| :--: | :-------------: | :---------- | :----------: |
| Standard input  | 0 | Data stream for input. A command pipeline.       | stdin  |
| Standard output | 1 | Data stream for output. User's screen.           | stdout |
| Standard output | 1 | Data stream for output the error. User's screen. | stderr |

### `read` function

Prototype:

```c
ssize_t read(int fd, void *buf, size_t count);
```

The function takes in three arguments.

1. `fd` - The file descriptor of the file
2. `buf` - The buffer where the read data is to be stored
3. `count` - number of bytes to read from the file

The value returned is the number of bytes read.

`read()` attempts to read `count` bytes of data from the file that is pointed by `fd` into the buffer pointed to by `buf`.

On objects capable of seeking, the `read()` starts at a position given by the pointer associated with `fd`. Upon return from `read()`, the pointer is incremented by the number of bytes actually read.

Imagine when we call `read()`, there will be a little cursor (pointer) before the first character when we call it for the first time. When we tell `read()`, "I want to read *n* bytes", let's say n is 5. So, it will advance its reading head (the cursor) by five bytes. Then, it will store that 5 bytes in a buffer that we will have sent to it. At the end, it will return the number of bytes it managed to read.

If it goes well, it will return the number of bytes that we ask it to read, in this case, 5. If the returned value is less than 5, meaning it reaches EOF.

### `static` variables

> Like global variable but with limited scope.

By default, when you declare a static variable, it will be initialized with a default value.

```c
static int x; // default value: 0
static char y; // default value: \0
static char *z; // default value: (null)
```

The value of a static variable will be cleared when the program ends. It's not stored on the stack. Static variables are allocated memory in data segment.

The lifetime of a static variable is the entire run of the program.

Double pointer: A pointer stores the address of another pointer.

Use double pointer because I want to alter or change the value of the stash inside the function. In C, you can either pass by value or reference. If you pass by value in a function call, then changes made in the function won't reflect at calling place.

Soft limit is the value of the current process limit that is enforced by the operating system. For, linux the soft limit for each process is 1024, 

![Memory layout of C](https://i0.wp.com/rjp.b44.myftpupload.com/wp-content/uploads/2019/12/image-9.png?resize=623%2C439)
