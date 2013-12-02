to compile:
-----------
cc search.c -o search `/usr/local/mnogosearch/bin/udm-config --libs` `/usr/local/mnogosearch/bin/udm-config --cflags`

to run:
-------
export LD_LIBRARY_PATH=/usr/local/mnogosearch/lib
and execute


Mnogosearch installation notes
------------------------------

indexer.conf:
# do not index hidden files
Disallow Regex \.[^/]*

I needed to add
Allow *.pdf
to make my pdf file indexed, event if they say that mnogosearch automatically add Allow regex .*

# Directory to index, do not forget the last /
Server path file:///var/www/

# if documents are stored in UTF-8
# set the default charset
#RemoteCharset UTF-8
# you can also add it to the parser config:
# see this: http://www.mnogosearch.org/doc33/msearch-parsers.html#pars-char
Mime application/pdf          "text/plain; charset=utf-8"                 "pdftotext $1 -"

apache:

    ScriptAlias /mno-bin/ /usr/local/mnogosearch/bin/
    <Directory "/usr/local/mnogosearch/bin/">
        AllowOverride None
        Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
        Order allow,deny
        Allow from all
    </Directory>


The mnogosearch/var directory is used when indexing and needs to be writable by the indexer process!

#-------

