For all source code changes, run clang-format using the following command prior to committing:

```
find . -regex '.*\.(cpp\|hpp\|cu\|cuh\|c\|h)' -exec clang-format -style=file -i {} \;
```

