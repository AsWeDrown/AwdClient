:: awd/net/packets.proto
RD /S /Q "E:\�஥���\Java\AsWeDrown\awd-server\build\generated\pbclasses\gg"
"E:\Program Files\protoc\bin\protoc.exe" -I="E:\CLionProjects\AsWeDrown\AwdClient\net" --java_out="E:\CLionProjects\AsWeDrown\AwdClient\net" "E:\CLionProjects\AsWeDrown\AwdClient\net\packets.proto"
DEL "E:\�஥���\Java\AsWeDrown\awd-server\src\main\proto\net\packets.proto"
COPY "E:\CLionProjects\AsWeDrown\AwdClient\net\packets.proto" "E:\�஥���\Java\AsWeDrown\awd-server\src\main\proto\net\packets.proto"
MOVE /Y "E:\CLionProjects\AsWeDrown\AwdClient\net\gg" "E:\�஥���\Java\AsWeDrown\awd-server\build\generated\pbclasses\gg"
