# Prepend

TBD: Test

Simple command line utility to insert some text in the
beginning of the file inplace.
The process requires prefix*2 buffer size.

The utility is an optimization if the command `sed -i '1i/sometext' [FILE]`, 
except that sed creates a temporary file copy. This utility does not.

USAGE : prepend PREFIXFILE FILE [FILE]*

WARNING: Process is not transactional. If interrupted in the middle, 
the file is left corrupted.

TODO :
 * Non-file sources for the prefix support.
 * Optimize for using smaller blocksize.
 * Create roll-back state file.
 * Insert at byte/line.
 * Check for prefix exists
 * Tests
 * Any requests?
 
