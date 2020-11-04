Contributions to the library are gladly accepted.


If you don't have time to submit a bug fix, please open an issue report on
GitHub so at least the problem is known. The same applies to improvements -
don't hesitate to open feature request so that the design of new APIs could
take your use case into account.

Documentation
-------------

If you find a problem in the documentation, please open an issue with a link to
the page in question.

Code changes
------------

Please test your code changes by running the following:

```
cd test
sh runAllTests.sh

# on ARM
sh runAllTests.sh arm
```

Please use the `master` branch as a target for pull requests. 
The rest are periodically tested on a private test farm.

Code style
----------

C-style pointer casts are discouraged, use reinterpret_cast for that. For
non-pointer types C-style casts are preferred over static_cast or constructor.
