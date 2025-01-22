def explore_text_encoding(text="Hello, 안녕"):
    """
    Demonstrates various text encoding concepts using a sample string.
    The string contains both ASCII and Unicode characters to show different encoding behaviors.
    """

    # 0. original text
    print("=== Original Text ===")
    print(text)
    print("\n")

    # 1. Convert string to different encodings
    utf8_encoded = text.encode('utf-8')
    utf16_encoded = text.encode('utf-16')
    ascii_encoded = text.encode('ascii', errors='replace')  # Replace non-ASCII chars with '?'

    # 2. Show raw bytes
    print("=== Raw Bytes ===")
    print(f"UTF-8:  {utf8_encoded}")
    print(f"UTF-16: {utf16_encoded}")
    print(f"ASCII:  {ascii_encoded}")

    # 3. Show byte counts
    print("\n=== Byte Lengths ===")
    print(f"UTF-8:  {len(utf8_encoded)} bytes")
    print(f"UTF-16: {len(utf16_encoded)} bytes")
    print(f"ASCII:  {len(ascii_encoded)} bytes")

    # 4. Show individual byte values in different formats
    print("\n=== Detailed Byte Analysis ===")
    print("UTF-8 bytes:")
    print("Decimal:", [b for b in utf8_encoded])
    print("Hex:    ", [hex(b) for b in utf8_encoded])
    print("Binary: ", [bin(b)[2:].zfill(8) for b in utf8_encoded])

    # 5. Demonstrate decoding
    print("\n=== Decoding Back to String ===")
    print(f"UTF-8  → String: {utf8_encoded.decode('utf-8')}")
    print(f"UTF-16 → String: {utf16_encoded.decode('utf-16')}")
    print(f"ASCII  → String: {ascii_encoded.decode('ascii')}")

def demonstrate_number_encoding():
    """
    Shows how numbers are encoded in different formats.
    """
    number = 42

    # Integer representations
    print("=== Number Encoding ===")
    print(f"Decimal: {number}")
    print(f"Binary:  {bin(number)}")
    print(f"Hex:    {hex(number)}")

    # Show binary layout for different integer sizes
    int8 = number.to_bytes(1, byteorder='big', signed=True)
    int16 = number.to_bytes(2, byteorder='big', signed=True)
    int32 = number.to_bytes(4, byteorder='big', signed=True)

    print("\n=== Integer Storage Sizes ===")
    print(f"8-bit:  {[bin(b)[2:].zfill(8) for b in int8]}")
    print(f"16-bit: {[bin(b)[2:].zfill(8) for b in int16]}")
    print(f"32-bit: {[bin(b)[2:].zfill(8) for b in int32]}")

    # Demonstrate endianness
    print("\n=== Endianness ===")
    number_bytes = number.to_bytes(4, byteorder='big')
    print(f"Big endian:    {[hex(b) for b in number_bytes]}")
    number_bytes = number.to_bytes(4, byteorder='little')
    print(f"Little endian: {[hex(b) for b in number_bytes]}")

if __name__ == "__main__":
    # Example usage
    print("Text Encoding Examples:")
    print("-" * 50)
    explore_text_encoding()

    print("\nNumber Encoding Examples:")
    print("-" * 50)
    demonstrate_number_encoding()
