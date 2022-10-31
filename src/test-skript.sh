#!/usr/bin/bash

cd tests/ && for TEST in *; do
diff --color -u <(tail -n +3 $TEST) <(../scanner-test <(head -n 1 $TEST))
done
    
