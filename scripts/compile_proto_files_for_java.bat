:: awd/net/packets.proto
RD /S /Q "E:\Проекты\Java\AsWeDrown\awd-server\build\generated\pbclasses\gg"
"E:\Program Files\protoc\bin\protoc.exe" -I="E:\CLionProjects\AsWeDrown\AwdClient\net" --java_out="E:\CLionProjects\AsWeDrown\AwdClient\net" "E:\CLionProjects\AsWeDrown\AwdClient\net\packets.proto"
DEL "E:\Проекты\Java\AsWeDrown\awd-server\src\main\proto\net\packets.proto"
COPY "E:\CLionProjects\AsWeDrown\AwdClient\net\packets.proto" "E:\Проекты\Java\AsWeDrown\awd-server\src\main\proto\net\packets.proto"
MOVE /Y "E:\CLionProjects\AsWeDrown\AwdClient\net\gg" "E:\Проекты\Java\AsWeDrown\awd-server\build\generated\pbclasses\gg"
