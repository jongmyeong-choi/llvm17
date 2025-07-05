with a, b : a*(4+b)

```
cheese@cheeMacMk2 src % cmake \
-GNinja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
-DLLVM_DIR=/Users/cheese/git/llvm-project/llvm ../
cheese@cheeMacMk2 src % ninja
```

```
cheese@cheeMacMk2 src % src/calc "with a:a*3;" | llc -filetype=obj -o expr.obj
cheese@cheeMacMk2 src % clang -o expr expr.obj rtcalc.c
cheese@cheeMacMk2 src % ./expr
Enter a value for a: 4
The result is 12
```