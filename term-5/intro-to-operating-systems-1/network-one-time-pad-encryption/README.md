This is a series of 5 programs, all of which can be compiled in linux by running the "compileall" script that is included.
The keygen program produces a key for one-time-pad (OTP) encryption. It takes an argument for the character length of the key
and its output should beredirected to a file. The length of the key should be at least as long as the plaintext that will be encrypted. otp_enc_d is a process that should be run in the background that can encrypt 
a plaintext using an OTP key. otp_enc_d must be given a port number to listen on and is called like so: otp_enc_d 57171 &.
Once otp_enc_d is running and an OTP key has been created with keygen, the program otp_enc can be run with plaintext file,
OTP key, and port number that otp_enc_d is listening on as arguments. Its output should be redirected to a file.
Like this: otp_enc plaintextFile keyFile, 57171 > cyphertextFile. The cyphertext is the encrypted version of the plaintext.
Decription follows a similar process with otp_dec_d running in the background, listening on a different port, and a call
to otp_dec like this: otp_dec cyphertextFile keyFile, 57172 > plaintextDecrypted.
