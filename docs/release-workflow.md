# Release Workflow

This project uses GitHub Actions to build release binaries for multiple Linux distributions. A workflow file `release.yml` defines the steps.

## Trigger

The workflow runs whenever a tag matching `v*.*.*` is pushed. A safeguard within
the workflow also verifies that the ref type is a tag so accidental branch
pushes will not trigger a release. Example:

```bash
git tag v1.0.0
git push origin v1.0.0
```

## Build matrix

Binaries are compiled on both **Ubuntu** and **Fedora** runners. Each job performs a standard `./configure && make` build.

## Packaging

After compilation the resulting `hx` binary is archived into `hx-ubuntu-latest-$TAG.tar.gz` or `hx-fedora-latest-$TAG.tar.gz` (where `$TAG` is the version tag such as `v1.0.0`).

## Release upload

The tarball from each job is uploaded as an asset on the GitHub Release automatically created for the tag. You will find two tarballs attached to the release page.

