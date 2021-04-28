: Этот файл нужно всегда запускать после изменения .proto файлов.
CALL "compile_proto_files_for_java.bat"
CALL "run-awd-ptrans-codegen.bat"
PAUSE
