#include "app.hpp"

App::App(int argc, char* argv[]) {
    parseArgs(argc, argv);

    if (inputFilePath.empty() && !interactiveMode && !showHelp) {
        throw std::invalid_argument("Input file not specified");
    }
}

void App::run() {
    if (showHelp) {
        printHelp();
        return;
    }

    // If neither output file nor showOutput is specified, save to default output file
    if (!hasOutputFile && !showOutput) {
        hasOutputFile = true;
        outputFilePath = "output.txt";
    }

    if (interactiveMode) {
        runInteractiveMode();
    } else {
        runFileMode();
    }
}

void App::runInteractiveMode() {
    // while loop to read input from user until terminateKeyword is entered
    std::string input;
    std::string inputContent;

    while (true) {
        std::cout << "Enter TINY code (type '" << terminateKeyword << "' to finish):\n";
        std::getline(std::cin, input);

        if (input == terminateKeyword) {
            break;
        }

        inputContent += input + "\n";
    }

    processTokens(inputContent);
}

void App::runFileMode() {
    // read input file
    std::string inputFileContent = FileHandler::readFile(inputFilePath);

    processTokens(inputFileContent);
}

void App::processTokens(std::string& inputFileContent) {
    // Empty input file content
    if (inputFileContent.empty()) {
        throw std::invalid_argument("Input file is empty");
        return;
    }

    // TODO: Uncomment the following code after implementing the Scanner and TokenStreamBuilder classes

    // initialize scanner
    // Scanner scanner(inputFileContent);

    // initialize tokenStreamBuilder
    // TokenStreamBuilder tokenStreamBuilder(scanner);

    // build token stream
    // tokenStreamBuilder.build();
    // std::vector<Token> tokens = tokenStreamBuilder.getTokens();

    // if showOutput is true, print tokens to console
    if (showOutput) {
        // printTokens(tokens, includeTokenPosition);
    }

    // write tokens to output file if specified
    if (!hasOutputFile) {
        // FileHandler::writeTokens(outputFilePath, tokens, includeTokenPosition);
    }
}

void App::printTokens(const std::vector<Token>& tokens, bool includePosition) {
    std::cout << "Tokens:\n";

    for (const Token& token : tokens) {
        std::cout << token.toString(includePosition) << std::endl;
    }
}

void App::printHelp() {
    std::cout << "Usage: scanner [options] [input_file [output_file]]\n"
              << "Options:\n"
              << "  -h, --help                      Show this help message\n"
              << "  -i, --input <file>              Specify the input file\n"
              << "  -o, --output <file>             Specify the output file\n"
              << "  -m, --mode <mode>               Specify the mode (file, interactive)\n"
              << "  -s, --show-output               Display output in the console\n"
              << "      --include-token-position    Include token position in output\n"
              << "  -t, --terminate-keyword <kw>    Termination keyword for interactive mode\n"
              << "      --default-output            Save to a default output file if not specified\n"
              << "\n"
              << "Examples:\n"
              << "  scanner input.txt output.txt\n"
              << "  scanner -i input.txt -o output.txt --show-output\n"
              << "  scanner --mode interactive\n"
              << "  scanner input.txt --show-output\n";
}

void App::parseArgs(int argc, char* argv[]) {
    // struct for long options
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"mode", required_argument, 0, 'm'},
        {"show-output", no_argument, 0, 's'},
        {"include-token-position", no_argument, 0, 0},
        {"terminate-keyword", required_argument, 0, 't'},
        {"default-output", no_argument, 0, 0},
        {0, 0, 0, 0}  // Terminate the option array
    };

    int option_index = 0;
    int c;

    // parse options
    while ((c = getopt_long(argc, argv, "hi:o:m:t:s", long_options, &option_index)) != -1) {
        switch (c) {
            case 0:
                if (long_options[option_index].flag == 0) {
                    break;
                }

                if (std::string(long_options[option_index].name) == "include-token-position") {
                    includeTokenPosition = true;
                } else if (std::string(long_options[option_index].name) == "default-output") {
                    outputFilePath = "output.txt";
                    hasOutputFile = false;
                }
                break;

            case 'h':
                showHelp = true;
                printHelp();
                break;

            case 'i':
                inputFilePath = optarg;
                break;

            case 'o':
                outputFilePath = optarg;
                hasOutputFile = false;
                break;

            case 'm':
                if (std::string(optarg) == "interactive") {
                    interactiveMode = true;
                } else if (std::string(optarg) == "file") {
                    interactiveMode = false;
                } else {
                    throw std::invalid_argument("Invalid mode specified. Use 'interactive' or 'file' as the mode.");
                }

                break;

            case 't':
                terminateKeyword = optarg;
                break;

            case 's':
                showOutput = true;
                break;

            case '?':
                throw std::invalid_argument("Invalid option specified, use -h or --help for usage information.");
                break;

            default:
                break;
        }
    }

    handlePositionalArgs(argc, argv);
}

void App::handlePositionalArgs(int argc, char* argv[]) {
    std::vector<std::string> positionalArgs;

    // get positional arguments
    while (optind < argc) {
        positionalArgs.push_back(argv[optind++]);
    }

    // assign positional arguments to input and output file paths
    if (inputFilePath.empty() && !positionalArgs.empty()) {
        inputFilePath = positionalArgs[0];
        if (outputFilePath.empty() && positionalArgs.size() > 1) {
            outputFilePath = positionalArgs[1];
            hasOutputFile = false;  // output file is specified
        }
    }

    // if no output file is specified and default output is enabled, set output file to "output.txt"
    if (outputFilePath.empty() && !hasOutputFile) {
        outputFilePath = "output.txt";
    }
}