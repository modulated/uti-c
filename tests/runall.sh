tests/bioseq.test
if [ $? -ne 0 ]; then exit;
fi

tests/random.test
if [ $? -ne 0 ]; then exit;
fi

tests/stats.test
if [ $? -ne 0 ]; then exit; 
fi