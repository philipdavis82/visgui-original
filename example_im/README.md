# Visgui Example

```bash
docker build --target build  -t visgui.example_im.build                --file example_im/Dockerfile .
docker build --target export -t philipdavis82/visgui.example_im.export --file example_im/Dockerfile .
```

Push

```bash
docker push philipdavis82/visgui.example_im.export
```
