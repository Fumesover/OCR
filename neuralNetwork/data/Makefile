

init:
	wget https://archive.ics.uci.edu/ml/machine-learning-databases/00417/fonts.zip
	unzip fonts.zip
	rm fonts.zip
	for filename in *.csv; do \
		grep -E "^[^,]*,[^,]*,(3[3-9]|[4-9][0-9]|1[0-1][0-9]|12[0-6])," "$$filename" > "$$filename".clean; \
	done
	rm *.csv
	for filename in *.csv.clean; do \
		echo $$filename; \
		python createinp.py -i "$$filename"; \
	done
	rm *.csv.clean
