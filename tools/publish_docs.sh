#!/bin/bash
# Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
(cd _build && make doc -j 4)
(cd _build && make rebuild_doc)
# the remote documentation host computer data
doc_user=${USER}
doc_host="kifri.fri.uniza.sk"
doc_home="./public_html/oglplus"
doc_base="${doc_home}/html"
# local path to docs
doc_src="$(dirname $0)/../_build/doc/doxygen/html"
doc_web="$(dirname $0)/../web"
#
# remove the old files
ssh ${doc_user}@${doc_host} "rm -rf ${doc_base}/*"
# tar stream and untar the new files
(cd ${doc_src} && tar -c . | ssh ${doc_user}@${doc_host} "mkdir -p ${doc_base} && cd ${doc_base} && tar -x && echo 'AddCharset UTF-8 .html' > .htaccess")
#
#scp ${doc_web}/* ${doc_user}@${doc_host}:${doc_home}/
