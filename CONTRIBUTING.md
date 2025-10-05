# Contributing to Computational Graphics Project

Thank you for your interest in contributing to the Computational Graphics Project! This document provides guidelines for contributing to the project.

## Code of Conduct

This project follows a code of conduct that ensures a welcoming environment for all contributors. Please be respectful and constructive in all interactions.

## Getting Started

### Prerequisites
- C++17 or later
- OpenGL 3.3+
- CMake 3.16+
- Git

### Development Setup
1. Fork the repository
2. Clone your fork: `git clone https://github.com/your-username/computational-graphics-project.git`
3. Create a feature branch: `git checkout -b feature/your-feature-name`
4. Set up the build environment following the main README instructions

## Contribution Guidelines

### Reporting Issues
- Use the GitHub issue tracker
- Provide detailed reproduction steps
- Include system information (OS, compiler, OpenGL version)
- Attach relevant code snippets or error messages

### Suggesting Enhancements
- Use the GitHub issue tracker with the "enhancement" label
- Describe the feature and its benefits
- Provide implementation ideas if possible
- Consider backward compatibility

### Code Contributions

#### Code Style
- Follow the existing code style and conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and small
- Use const correctness

#### Commit Messages
- Use clear, descriptive commit messages
- Start with a verb in imperative mood
- Include a brief description of changes
- Reference issues when applicable

Example:
```
Add shadow mapping support for directional lights

- Implement ShadowMapper class with depth buffer rendering
- Add light space matrix calculations
- Include shadow bias and distance controls
- Fixes #123
```

#### Testing
- Add unit tests for new functionality
- Ensure all existing tests pass
- Test on multiple platforms if possible
- Update documentation as needed

### Pull Request Process

1. **Fork and Branch**: Create a feature branch from main
2. **Develop**: Implement your changes with tests
3. **Test**: Run the test suite and ensure all tests pass
4. **Document**: Update documentation if needed
5. **Submit**: Create a pull request with a clear description

#### Pull Request Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Unit tests added/updated
- [ ] Manual testing performed
- [ ] All tests pass

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No breaking changes (or clearly documented)
```

## Development Areas

### High Priority
- Performance optimizations
- Cross-platform compatibility
- Additional lighting models
- Post-processing effects
- VR/AR support

### Medium Priority
- Additional shader effects
- Asset loading improvements
- UI system
- Scene serialization
- Animation system

### Low Priority
- Additional test coverage
- Documentation improvements
- Code refactoring
- Build system improvements

## Technical Guidelines

### Architecture
- Follow the existing component-based architecture
- Maintain separation of concerns
- Use smart pointers for memory management
- Implement RAII principles

### Performance
- Profile code before and after changes
- Use the PerformanceMonitor for measurements
- Consider GPU memory usage
- Optimize for 60+ FPS target

### OpenGL Best Practices
- Use modern OpenGL (3.3+)
- Implement proper error checking
- Use VAOs for vertex data
- Minimize state changes
- Use appropriate data types

## Review Process

### Code Review Checklist
- [ ] Code follows style guidelines
- [ ] Logic is correct and efficient
- [ ] Error handling is appropriate
- [ ] Documentation is updated
- [ ] Tests are included
- [ ] No memory leaks
- [ ] Performance impact considered

### Review Timeline
- Initial review within 48 hours
- Follow-up reviews as needed
- Merge within 1 week for approved changes

## Release Process

### Version Numbering
- Major.Minor.Patch (e.g., 1.2.3)
- Major: Breaking changes
- Minor: New features
- Patch: Bug fixes

### Release Checklist
- [ ] All tests pass
- [ ] Documentation updated
- [ ] Version numbers updated
- [ ] Changelog updated
- [ ] Release notes prepared

## Community

### Communication
- Use GitHub issues for discussions
- Be respectful and constructive
- Help others when possible
- Share knowledge and learnings

### Recognition
- Contributors will be credited in the README
- Significant contributions may be highlighted in release notes
- Long-term contributors may be invited as maintainers

## Questions?

If you have questions about contributing, please:
1. Check existing issues and discussions
2. Create a new issue with the "question" label
3. Contact the maintainers directly

Thank you for contributing to the Computational Graphics Project!
