# csv2xml-with-C
Dynamic Converter by using C Language

CSV2XML tool must take the following command line arguments:
○ -separator
defines separator between each datum (1=comma, 2=tab, 3=semicolon)
○ -opsys
defines the end of line character format (1=windows, 2=linux, 3=macos)
○ -h
print information to screen about how other arguments can be used.

The sample command line usage converting from CSV to XML as follows:
CVS2XML Contacts.csv Contacts.xml -separator 1 -opsys 1
