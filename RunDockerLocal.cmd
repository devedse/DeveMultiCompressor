docker build -t devemulticomp -f DeveMultiCompressor.ConsoleApp/Dockerfile .
rem docker run --rm -it -v %cd%/DeveMultiCompressor.Tests/TestFiles:/testfiles --entrypoint /bin/bash devemulticomp
docker run --rm -it -v %cd%/DeveMultiCompressor.Tests/TestFiles:/testfiles devemulticomp -i /testfiles/enwik4.txt -v --include fast

rem ./DeveMultiCompressor.ConsoleApp -i /testfiles/enwik4.txt -v --include fast