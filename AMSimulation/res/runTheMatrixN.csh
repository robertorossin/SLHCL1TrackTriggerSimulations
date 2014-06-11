mkdir -p runTheMatrixN && cd runTheMatrixN

mkdir -p n5  && sed "s@26000000@5000000@"  ../runTheMatrix.csh >! n5/runTheMatrix.csh
mkdir -p n10 && sed "s@26000000@10000000@" ../runTheMatrix.csh >! n10/runTheMatrix.csh
mkdir -p n15 && sed "s@26000000@15000000@" ../runTheMatrix.csh >! n15/runTheMatrix.csh
mkdir -p n18 && sed "s@26000000@18000000@" ../runTheMatrix.csh >! n18/runTheMatrix.csh
mkdir -p n20 && sed "s@26000000@20000000@" ../runTheMatrix.csh >! n20/runTheMatrix.csh
mkdir -p n22 && sed "s@26000000@22000000@" ../runTheMatrix.csh >! n22/runTheMatrix.csh
mkdir -p n24 && sed "s@26000000@24000000@" ../runTheMatrix.csh >! n24/runTheMatrix.csh
mkdir -p n26 && sed "s@26000000@26000000@" ../runTheMatrix.csh >! n26/runTheMatrix.csh

cd -
echo "source runTheMatrixN/n5/runTheMatrix.csh  >! runTheMatrixN/n5/runTheMatrix.log  &"
echo "source runTheMatrixN/n10/runTheMatrix.csh >! runTheMatrixN/n10/runTheMatrix.log &"
echo "source runTheMatrixN/n15/runTheMatrix.csh >! runTheMatrixN/n15/runTheMatrix.log &"
echo "source runTheMatrixN/n18/runTheMatrix.csh >! runTheMatrixN/n18/runTheMatrix.log &"
echo "source runTheMatrixN/n20/runTheMatrix.csh >! runTheMatrixN/n20/runTheMatrix.log &"
echo "source runTheMatrixN/n22/runTheMatrix.csh >! runTheMatrixN/n22/runTheMatrix.log &"
echo "source runTheMatrixN/n24/runTheMatrix.csh >! runTheMatrixN/n24/runTheMatrix.log &"
echo "source runTheMatrixN/n26/runTheMatrix.csh >! runTheMatrixN/n26/runTheMatrix.log &"

