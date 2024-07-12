# Visgui Example

```bash
docker build --target build  -t visgui.example.build                --file example/Dockerfile .
docker build --target export -t philipdavis82/visgui.example.export --file example/Dockerfile .
```

push

```bash
docker push philipdavis82/visgui.example.export
```