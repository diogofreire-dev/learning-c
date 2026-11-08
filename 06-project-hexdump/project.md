# 06 – Project: a `hexdump` clone

A clone of `hexdump -C` that shows the bytes of any file exactly as they are stored.

```
00000000  23 69 6e 63 6c 75 64 65  20 3c 73 74 64 69 6f 2e  |#include <stdio.|
└offset┘  └──────────── 16 bytes in hex, 8 + 8 ────────────┘  └─ the same as text ─┘
```

## Usage

```bash
make run/06-project-hexdump/hexdump ARGS="01-basics/00-hello.c"
make run/06-project-hexdump/hexdump ARGS="-s 0x10 -n 32 01-basics/00-hello.c"
printf 'hi\n' | ./build/06-project-hexdump/hexdump
```

| Option | Meaning |
|---|---|
| `-s OFFSET` | skip `OFFSET` bytes (decimal, or hex with `0x`) |
| `-n LENGTH` | show at most `LENGTH` bytes |
| `-v` | show every line (no `*` for repeated lines) |
| `FILE` | file to read; without it, read standard input |

## Requirements (all done ✅)

- [x] Same output as `hexdump -C`, byte for byte
- [x] 16 bytes per line, extra space after the 8th, non-printable bytes as `.`
- [x] Short last line keeps the `|...|` column aligned
- [x] Repeated full lines replaced by a single `*` (unless `-v`)
- [x] Final line with the total offset; nothing at all for an empty input or `-n 0`
- [x] `-s`, `-n`, `-v`, reading from a file or from stdin
- [x] Errors to `stderr` with exit code 1 (missing file, invalid number, unknown option)
- [x] No memory leaks

**Testing:** the output was compared with the real `hexdump -C` using `diff` – **136 out of 136 identical**, on empty files, files full of zeros, random bytes, text, compiled programs (including `/bin/ls`), stdin and every combination of `-s`, `-n` and `-v`.

```bash
diff <(hexdump -C FILE) <(./build/06-project-hexdump/hexdump FILE) && echo identical
```

## Roadmap: from hexdump to a malware triage tool

The goal: a tool that looks at a suspicious file **without running it** (static analysis) and points out what deserves attention. Each phase is a new C skill.

### Phase 2 – What kind of file is it? (magic bytes)

Most formats start with a fixed signature:

| Bytes | Format |
|---|---|
| `4d 5a` (`MZ`) | Windows executable (PE: `.exe`, `.dll`) |
| `7f 45 4c 46` (`.ELF`) | Linux executable |
| `cf fa ed fe` | macOS executable (Mach-O 64-bit) |
| `ca fe ba be` | macOS universal binary / Java class |
| `50 4b 03 04` (`PK`) | ZIP – also `.docx`, `.xlsx`, `.jar`, `.apk` |
| `d0 cf 11 e0` | old Office documents (`.doc`, `.xls` – classic macro malware) |
| `25 50 44 46` (`%PDF`) | PDF |
| `89 50 4e 47` | PNG |
| `ff d8 ff` | JPEG |

🚩 Flag a **mismatch**: `invoice.pdf` that starts with `MZ` is a Windows program in disguise.

### Phase 3 – Strings (like the `strings` command)

- Extract every run of 4+ printable characters, and also UTF-16 strings (Windows stores text as `H.e.l.l.o.`).
- 🚩 Highlight suspicious ones: URLs and IPs (command-and-control servers), `cmd.exe`, `powershell -enc`, registry `Run` keys (persistence), long Base64 blobs.

### Phase 4 – Entropy (is it packed or encrypted?)

- **Shannon entropy** measures how random the bytes are: 0 = all the same, 8 = completely random.
- Normal code and text: about 4–6. Compressed or encrypted data: close to 8.
- Show the entropy of each block of the file as a small bar graph. 🚩 A program that is almost all high entropy is probably **packed** (its real code is hidden and only unpacked when it runs) – a common malware trick. Also look for packer signatures such as `UPX!`.

### Phase 5 – Hashes

- Implement **SHA-256** by hand (great practice with bitwise operators and `uint32_t`).
- The hash identifies a file uniquely: it can be searched on VirusTotal or MalwareBazaar to see whether the file is already known as malware – without uploading it.

### Phase 6 – Reading executable headers (ELF / PE)

- Parse the header with `struct`s (topic 05) to list the **sections** and their entropy, and the **imported functions**.
- 🚩 Suspicious Windows imports: `VirtualAlloc` + `WriteProcessMemory` + `CreateRemoteThread` (code injection), `IsDebuggerPresent` (anti-analysis), `URLDownloadToFile`.

### Phase 7 – Signatures (a mini YARA)

- Read rules from a file (`name: hex pattern`, with `??` as a wildcard byte) and report which rules match.
- That is how antivirus signatures and **YARA**, the standard tool of malware analysts, work.

### Safety rules ⚠️

- **Static analysis only: never run a sample.** The tool only reads bytes.
- Start with **harmless** files: your own programs, system binaries, and the **EICAR test file** (a standard string that every antivirus detects but that does nothing).
- Real samples (e.g. from MalwareBazaar) come in password-protected ZIPs (password `infected`). Only open them inside an **isolated virtual machine** with no shared folders – most samples target Windows, but never take the risk on your own computer.
