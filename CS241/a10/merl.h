#ifndef A10P5merl
#define A10P5merl

#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

// Struct for entries in the MERL footer.
struct Entry
{
  // Type of entry.
  enum Type
  {
    REL = 0x01,
    ESD = 0x05,
    ESR = 0x11
  } type;
  // Address to be relocated for REL entries, or label location for ESD/ESR entries.
  int location;
  // Name of ESD/ESR label (blank for REL entries).
  std::string name;

  // Returns whether the entry has a "name" field".
  bool hasName() const;

  // Returns the size of the entry in bytes.
  int size() const;

  // Prints a human-readable representation of the entry.
  // This is NOT the same as writing the entry out as raw binary data. This function is provided
  // for debugging purposes only. Use operator<< to write the binary representation of the entry.
  void print(std::ostream &os) const;

  // Constructs an entry from the given type, location and (optionally) name.
  Entry(Entry::Type type, int location, std::string name = "");

  // Reads one entry from the given input stream.
  // Throws std::runtime_error if the first word in the entry is not a valid entry type, that is,
  // if it is something other than 0x1 (REL), 0x5 (ESD), or 0x11 (ESR).
  Entry(std::istream &is);
};

// Writes a MERL entry (in binary) to an output stream.
std::ostream &operator<<(std::ostream &os, const Entry &e);

// Struct for MERL files.
struct MERL
{
  // The MERL "cookie" that appears as the first word in the MERL header.
  const int cookie = 0x10000002;

  // The total size in bytes of the MERL file.
  // This is the second word in the MERL header.
  int endModule = 0;

  // Returns the address in the MERL file where the code segment ends.
  // This is the size in bytes of the code segment, plus twelve (the size in bytes of the header).
  // This is the third word in the MERL header.
  int endCode = 0;

  // The MIPS code portion of the MERL file.
  // Each element of the vector represents one word of the program.
  // Note that the header is NOT included in this vector.
  std::vector<int> code;

  // External symbol table (MERL footer).
  std::vector<Entry> table;

  // Writes a human-readable representation of the MERL file to the given output stream.
  // This is NOT the same as writing the MERL file out as raw binary data. This function
  // is provided for debugging purposes only. Use operator<< to write the MERL file as binary.
  void print(std::ostream &os) const;

  // Constructs a MERL file object from an input stream.
  // Throws std::runtime error in the following cases:
  // - The first word read is not equal to the "MERL cookie" (0x10000002).
  // - The file ends prematurely, before the number of bytes indicated by the second word read
  //   (which is expected to be the total size of the file).
  // - The footer contains an invalid entry.
  MERL(std::istream &is);

  // "Linking constructor" which constructs a new MERL file by linking the two given MERL files.
  // You are expected to implement this constructor. An implementation is not provided by merl.o.
  // This constructor is allowed to modify the given MERL files m1 and m2.
  MERL(MERL &m1, MERL &m2);
};

// Writes a MERL object (as a binary file) to an output stream.
std::ostream &operator<<(std::ostream &os, const MERL &merl);

#endif
