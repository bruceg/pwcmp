Name: @PACKAGE@
Summary: Password Comparison Module Framework
Version: @VERSION@
Release: 1
Copyright: GPL
Group: Utilities/System
Source: http://em.ca/~bruceg/@PACKAGE@/@PACKAGE@-@VERSION@.tar.gz
BuildRoot: %{_tmppath}/@PACKAGE@-buildroot
#BuildArch: noarch
URL: http://em.ca/~bruceg/@PACKAGE@/
Packager: Bruce Guenter <bruceg@em.ca>

%description
A framework for opaque comparison of passwords.

%prep
%setup

%build
echo gcc "%{optflags}" >conf-cc
echo gcc -s >conf-ld
echo %{_bindir} >conf-bin
make

%install
rm -fr %{buildroot}
rm -f installer.o installer instcheck
echo %{buildroot}%{_bindir} >conf-bin
make installer instcheck

mkdir -p %{buildroot}%{_bindir}
./installer
./instcheck

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc ANNOUNCEMENT COPYING NEWS README TODO *.html
%{_bindir}/*
