install:
	pip install --upgrade pip &&\
		pip install -r requirements.txt

test:
	python -m pytest -vv test_Basic_code.py

format:
	back *.py


lint:
	pylint --disable=R,C Basic_code.py

all: install lint test