#! /bin/bash
if [ ! -e /root/test/logical ]
then
	touch logical
elif [ -f /root/test/logical ]
then
	rm -f /root/test/logical && mkdir /root/test/logical
elif [ -d /root/test/logical ]
then
	rm -r /root/test/logical
fi

