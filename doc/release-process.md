Release Process
====================

Before any code can be accepted into Bethel Core a Release Candidate branch and PR must be presented to the community for a minimum stand-down period - as detailed below.

### Release Candidates

Release candidates are critical to the Bethel release eco-system and give the community and interested parties time to review the code and potentially prepare for any changes that may be introduced.  

#### Release Candidates and Release Version Convention

Bethel follows the Semantic Versioning.

e.g `v(MAJOR).(MINOR).(PATCH)` = `v4.2.1`

1. MAJOR version when you make incompatible changes - This applies to hard forks or any type of breaking changes,
2. MINOR version when you add functionality in a backward-compatible manner - This also applies to any soft forks.
3. PATCH version when you make backward-compatible bug fixes.

When preparing an RC branch and PR the versioning should have `-rc.(release)` appended to the end.

E.g: `v4.5.0-rc.1`

Every time a new release branch is cut the RC version should be increased to help inform the community.

E.g: `v2.5.0-rc.1`, `v2.5.0-rc.2`, `v2.5.0-rc.3`

#### Release Candidates Review Period

* PATCH - 2 Weeks minimum from the time of notification to the community, via signaling by the PR date.

* MINOR - 4 Weeks minimum from the time of notification to the community, via signaling by the PR date.

* MAJOR - 8 Weeks minimum from the time of notification to the community, via signaling by the PR date.

Please note any type of network security or stability issues will be prioritized and might not have any applied stand-down period.


#### Release Candidates Preparation

Before every release candidate:

* Update translations see [translation_process.md](https://github.com/bethel/bethel-core/blob/master/doc/translation_process.md#synchronising-translations).

Before every minor and major release:

* Update [npips.md](npips.md) to account for the protocol changes since the last release
* Update version in sources (see below)
* Write release notes (see below)

Before every major release:

* Update hardcoded [seeds](/contrib/seeds/README.md).

### First time / New builders

Check out the source code in the following directory hierarchy.

    cd /path/to/your/toplevel/build
    git clone https://github.com/bethel/bethel-core.git

### Bethel maintainers/release engineers, update version in sources

Update the following:

- `configure.ac`:
    - `_CLIENT_VERSION_MAJOR`
    - `_CLIENT_VERSION_MINOR`
    - `_CLIENT_VERSION_REVISION`
    - Don't forget to set `_CLIENT_VERSION_IS_RELEASE` to `true`, `_CLIENT_BUILD_IS_TEST_RELEASE` to `false` and `_CLIENT_BUILD_IS_RELEASE_CANDIDATE` to `false`
- `src/clientversion.h`: (this mirrors `configure.ac` - see issue #3539)
    - `CLIENT_VERSION_MAJOR`
    - `CLIENT_VERSION_MINOR`
    - `CLIENT_VERSION_REVISION`
    - Don't forget to set `CLIENT_VERSION_IS_RELEASE` to `true`
- `doc/README.md` and `doc/README_windows.txt`
- `doc/Doxyfile`: `PROJECT_NUMBER` contains the full version
- `contrib/gitian-descriptors/*.yml`: usually one'd want to do this on master after branching off the release - but be sure to at least do it before a new major release

Write release notes. git shortlog helps a lot, for example:

    git shortlog --no-merges v(current version, e.g. 4.0.6)..v(new version, e.g. 4.1.0)

Generate list of authors:

    git log --format='%aN' "$*" | sort -ui | sed -e 's/^/- /'

Tag version (or release candidate) in git

    git tag -s v(new version, e.g. 4.1.0)

### Setup and perform Gitian builds

Setup Gitian descriptors:

    pushd ./bethel-core
    export VERSION=(new version, e.g. v4.1.0, which should also be the name of the repository branch)
    git fetch
    git checkout v${VERSION}
    popd

Ensure gitian-builder is up-to-date:

    pushd ./gitian-builder
    git pull
    popd

### Fetch and create inputs: (first time, or when dependency versions change)

    pushd ./gitian-builder
    mkdir -p inputs
    wget -P inputs https://bitcoincore.org/cfields/osslsigncode-Backports-to-1.7.1.patch
    wget -P inputs http://downloads.sourceforge.net/project/osslsigncode/osslsigncode/osslsigncode-1.7.1.tar.gz
    popd

Create the OS X SDK tarball, see the [OS X readme](README_osx.md) for details, and copy it into the inputs directory.

### Optional: Seed the Gitian sources cache and offline git repositories

By default, Gitian will fetch source files as needed. To cache them ahead of time:

    pushd ./gitian-builder
    make -C ../bethel-core/depends download SOURCES_PATH=`pwd`/cache/common
    popd

Only missing files will be fetched, so this is safe to re-run for each build.

NOTE: Offline builds must use the --url flag to ensure Gitian fetches only from local URLs. For example:

    pushd ./gitian-builder
    ./bin/gbuild --url bethel-core=/path/to/bethel,signature=/path/to/sigs {rest of arguments}
    popd

The gbuild invocations below <b>DO NOT DO THIS</b> by default.

### Build and sign Bethel Core for Linux, Windows, and OS X:

    pushd ./gitian-builder
    ./bin/gbuild --memory 3000 --commit bethel-core=${VERSION} ../bethel-core/contrib/gitian-descriptors/gitian-arm.yml
    mv build/out/bethel-*.tar.gz build/out/src/bethel-*.tar.gz ../
    
    ./bin/gbuild --memory 3000 --commit bethel-core=${VERSION} ../bethel-core/contrib/gitian-descriptors/gitian-linux.yml
    mv build/out/bethel-*.tar.gz build/out/src/bethel-*.tar.gz ../

    ./bin/gbuild --memory 3000 --commit bethel-core=${VERSION} ../bethel-core/contrib/gitian-descriptors/gitian-win.yml
    mv build/out/bethel-*-win-unsigned.tar.gz inputs/bethel-win-unsigned.tar.gz
    mv build/out/bethel-*.zip build/out/bethel-*.exe ../

    ./bin/gbuild --memory 3000 --commit bethel-core=${VERSION} ../bethel-core/contrib/gitian-descriptors/gitian-osx.yml
    mv build/out/bethel-*-osx-unsigned.tar.gz inputs/bethel-osx-unsigned.tar.gz
    mv build/out/bethel-*.tar.gz build/out/bethel-*.dmg ../
    popd

Build output expected:

  1. source tarball (`bethel-${VERSION}.tar.gz`)
  2. linux 32-bit and 64-bit dist tarballs (`bethel-${VERSION}-linux[32|64].tar.gz`)
  3. windows 32-bit and 64-bit unsigned installers and dist zips (`bethel-${VERSION}-win[32|64]-setup-unsigned.exe`, `bethel-${VERSION}-win[32|64].zip`)
  4. OS X unsigned installer and dist tarball (`bethel-${VERSION}-osx-unsigned.dmg`, `bethel-${VERSION}-osx64.tar.gz`)
  5. Gitian signatures (in `gitian.sigs/${VERSION}-<linux|{win,osx}-unsigned>/(your Gitian key)/`)


The list of files should be:
```
bethel-${VERSION}-aarch64-linux-gnu.tar.gz
bethel-${VERSION}-arm-linux-gnueabihf.tar.gz
bethel-${VERSION}-i686-pc-linux-gnu.tar.gz
bethel-${VERSION}-x86_64-linux-gnu.tar.gz
bethel-${VERSION}-osx64.tar.gz
bethel-${VERSION}-osx.dmg
bethel-${VERSION}.tar.gz
bethel-${VERSION}-win32-setup.exe
bethel-${VERSION}-win32.zip
bethel-${VERSION}-win64-setup.exe
bethel-${VERSION}-win64.zip
```
The `*-debug*` files generated by the gitian build contain debug symbols
for troubleshooting by developers. It is assumed that anyone that is interested
in debugging can run gitian to generate the files for themselves. To avoid
end-user confusion about which file to pick, as well as save storage
space *do not upload these to the bethel.org server, nor put them in the torrent*.

- GPG-sign it, delete the unsigned file:
```
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)
Note: check that SHA256SUMS itself doesn't end up in SHA256SUMS, which is a spurious/nonsensical entry.

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the bethel.org server
  into `/var/www/bin/bethel-core-${VERSION}`

- A `.torrent` will appear in the directory after a few minutes. Optionally help seed this torrent. To get the `magnet:` URI use:
```bash
transmission-show -m <torrent file>
```
Insert the magnet URI into the announcement sent to mailing lists. This permits
people without access to `bethel.org` to download the binary distribution.
Also put it into the `optional_magnetlink:` slot in the YAML file for
bethel.org (see below for bethel.org update instructions).

- Update bethel.org version

- Announce the release:

-- Communicate with enough time to economic actors of the Network (exchanges):

- Celebrate
