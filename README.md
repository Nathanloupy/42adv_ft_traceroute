# ft_traceroute

This project is about recoding the traceroute command, which traces the route taken by packets across an IP network. It takes the version of inetutils-2.0 as a reference

### Usage

```bash
$> make
$> ./ft_traceroute --help
Usage: ft_traceroute [OPTIONS]

Print the route packets trace to network host.

Options:
  -f, --first-hop N        set initial hop distance, i.e. time-to-live, to N
  -m, --max-hop N          set maximal hop count to N (default: 64)
  -q, --tries N            send N probe packets per hop (default: 3)
  -r, --resolve-hostnames  resolve hostnames
  -w, --wait N             wait N seconds for response (default: 3)
  -h, --help               give this help list
      --usage              give a short usage message
```

### Examples

```bash
# Basic traceroute
$> ./ft_traceroute google.com

# Traceroute with hostname resolution
$> ./ft_traceroute -r google.com

# Custom maximum hops with faster timeout
$> ./ft_traceroute -m 20 -w 1 8.8.8.8

# Start from a specific hop with more tries per hop
$> ./ft_traceroute -f 5 -q 5 example.com

# Comprehensive trace with all options
$> ./ft_traceroute -r -f 1 -m 30 -q 3 -w 2 www.github.com
```

## Grading

This project is part of the advanced curriculum at 42 School.

- **Date of completion:** 2025-08-04
- **Grade:** 125/100