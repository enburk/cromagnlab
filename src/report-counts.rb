# Reporting the most popular articles
# Tim Bray, 2006

counts = {}
counts.default = 0

ARGF.each_line do |line|
  if line =~ %r{GET /ongoing/When/\d\d\dx/(\d\d\d\d/\d\d/\d\d/[^ .]+) }
    counts[$1] += 1
  end
end

keys_by_count = counts.keys.sort { |a, b| counts[b] <=> counts[a] }
keys_by_count[0 .. 9].each do |key|
  puts "#{counts[key]}: #{key}"
end

# Input (weblog):
# c80-216-32-218.cm-upc.chello.se - - [08/Oct/2006:06:37:48 -0700] "GET /ongoing/When/
# 200x/2006/10/08/Grief-Lessons HTTP/1.1" 200 5945 "http://www.tbray.org/ongoing/"
# "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322)
# ...
# Output:
# 4765: 2006/12/11/Mac-Crash
# 3138: 2006/01/31/Data-Protection
# 1865: 2006/12/10/EMail
# 1650: 2006/03/30/Teacup
# 1645: 2006/12/11/Java
# ...

# Beautiful Code, O'Reilly, 2007
