# Introduction and usage of camp2ascii

## What is it?

The data loggers made by Campbell Scientific store their data in a proprietary binary file format. The physical support is either a memory card or it is transmitted via a GSM modem, etc 
The tools presented here is an open source tool (covered by the [GPL license](http://www.fsf.org/licensing/licenses/gpl.html)) that allows to extract the data contained within these binary files. The output can be formatted to suit your needs and you can very precisely control how much data you want to extract

## How to use it?

This is a command line program. Running it with the "-h" option shows a quick help explaining which options are available. Namely, the following options can be used:

    -c Compatibility: produces the same amount of output as tob32
    -a Extract data from all valid frames
    -d [delimiter] Use [delimiter] as a field delimiter in the output
    -h Show this help
    -l Show the license
    -n Number of invalid frames triggering a full stop (0=never stop)
    -t Style to use for the output (grace, tob32, excel, sql)
    -o [output] Output data to file [output]
    -s Sort the output in chronological order (if necessary) (TODO)
    -p Use stdin as input data
    -k Do not output sub-seconds if equal zero
    -z Copy valid frames to the output, as binary

For example, to extract data from file mydata.tob using the default setup, use the following command line:

    camp2ascii mydata.tob -o mydata.asc

# TOB data structure

Some explanations might be needed... First, you have to know that all data is enclosed within frames in such a binary file. A frame contains a header (frame timestamp, etc), a data segment (the data we are interested in) and a footer (flags, validation code...). A frame might contain several physical timestamps, they will be calculated by camp2ascii if needed. At first, when inserting a new memory card, the data logger fills the card with one big file that contains only empty/invalid frames. Then, when doing the data acquisition, the frames are filled and marked as valid frames. If the memory card gets full, the data logger will write over the begining of the file. The "-c" option forces the extraction of frames according to what the original tool would do (thus not outputing all valid frames). On the other hand, the tools usualy given by Campbell Scientific don't output all the frames, specifically when the begining of the file has been written over. Because extracting all the frames would show the (potentially) newest data first (at the begining of the file) and then the older one (not yet written over by the data logger), the "-s" option allows you to ask for a sorted extraction: the old section of the file is extracted first, then a new pass is done in order to extract the newest section of the file.

# Sources:

http://models.slf.ch/p/camp2ascii/page/Usage/

http://mathias.bavay.free.fr/software/camp2ascii/index.html
