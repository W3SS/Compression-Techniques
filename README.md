
-shannon-fano.cpp

This program performs shannon fano coder.
The user needs to enter the input filename and output file name

To compile th code use
g++ -o Main shannon-fano.cpp bitio.cpp errhand.c

To execute use
./Main image.dat compressed_image.dat



-huffman.cpp

This program performs huffman coding.
The user needs to enter the input filename and output file name

To compile th code use
g++ -o Main huffman.cpp bitio.cpp errhand.c

To execute use
./Main image.dat compressed_image.dat


-RLC.cpp

This program performs RLC coder.
The user needs to enter the input filename and output file name

To compile th code use
g++ -o Main RLC.cpp bitio.cpp errhand.c

To execute use
./Main text.dat compressed_text.dat



-RLC_decoder.cpp

This program performs RLC de-coder.
The user needs to enter the input filename and output file name

To compile th code use
g++ -o Main RLC_decoder.cpp bitio.cpp errhand.c

To execute use
./Main compressed_text.dat decompressed_text.cpp




-modified_RLC.cpp

This program performs modified RLC coder.
The user needs to enter the input filename and output file name

To compile th code use
g++ -o Main modified_RLC.cpp bitio.cpp errhand.c

To execute use
./Main text.dat compressed_text.dat



-modified_RLC_decoder.cpp

This program performs modified_RLC de-coder.
The user needs to enter the input filename and output file name

To compile th code use
g++ -o Main modified_RLC_decoder.cpp bitio.cpp errhand.c

To execute use
./Main compressed_text.dat decompressed_text.cpp



-Vector_quantization.cpp

This program performs Vector quantization coding.
The user needs to enter the 3 training input filenames, codebook_size and block dim

To compile th code use
g++ Vector_quantization.cpp bitio.cpp errhand.c -o vq

To execute use
./vq chem.256 house.256 moon.256 64 2



-scalar_quantizer.cpp

This program performs scalar_quantizer coder.
The user needs to enter the 3 training input filenames and codebook_size

To compile th code use
g++ -o sq scalar_quantizer.cpp bitio.cpp errhand.c

To execute use
./sq moon.256 chem.256 house.256 16




-PSNR.cpp

This program evaluates PSNR.
The user needs to enter the original input filename, compressed file name and no. of channels

To compile th code use
g++ PSNR.cpp -o PSNR

To execute use
./PSNR p5.raw pepper.raw 3




