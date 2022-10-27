#!/usr/bin/bash

cd tests/ && for TEST in *; do
diff --color -u <(head -n 1 $TEST) <(../scanner-test <(tail -n +3 $TEST))
done
    
